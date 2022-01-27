#include <cassert>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"


LongEdgeFirstRelaysAlg::LongEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c)
: LongShortEdgeRelaysAlg(MST_graph, r_c, theta_c) {}

RelaysMSTGraph* LongEdgeFirstRelaysAlg::solve() {
    // Step 1: Connect relay nodes on long edges
    std::cout << "Step 1: Connect relay nodes on long edges\n";
    for(Edge* long_or_medium_edge : long_or_medium_edges_) {
        SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result = steinerizeLongOrMediumEdge(
            long_or_medium_edge, graph_node_type_);
    }

    // Step 2: Connect terminals to type-1 relay nodes
    std::cout << "Step 2: Connect terminals to type-1 relay nodes\n";
    for(Node* terminal : terminals_) {
        MSTNodeLam* terminal_1 = (MSTNodeLam*) terminal;
        if (terminal_1->type_1_relays_.empty()) continue;
        std::vector<Edge*> new_communication_edges = connectTerminalWithType1Relays(terminal_1);
        communication_edges_ += new_communication_edges;
    }

    // Step 3: Connect terminals on short edges
    std::cout << "Step 3: Connect terminals on short edges\n";
    for(Edge* short_edge : short_edges_) {
        SteinerizeShortEdgeResult_LEF steinerize_short_edge_result = steinerizeShortEdge(short_edge, graph_node_type_);
        relays_ += steinerize_short_edge_result.type_4_relays;
        communication_edges_ += steinerize_short_edge_result.communication_edges;
    }

    int n_type1_relays = 0;
    int n_type2_relays = 0;
    int n_type4_relays = 0;
    for(Node* relay : relays_) {
        if (relay->getNodeType() == RELAY_DD_NODE_TYPE_1) ++n_type1_relays;
        else if (relay->getNodeType() == RELAY_DD_NODE_TYPE_2) ++n_type2_relays;
        else if (relay->getNodeType() == RELAY_DD_NODE_TYPE_4) ++n_type4_relays;
    }
    assert(n_type1_relays + n_type2_relays + n_type4_relays == relays_.size());
    int n = terminals_.size();
    std::cout << "Check size: " << n << '\n';
    std::cout << "Type 1: " << n_type1_relays << ' ' << 2*long_or_medium_edges_.size() << '\n';
    std::cout << "Type 2: " << n_type2_relays << ' ' << 3*(n_total_nodes_omni_ - terminals_.size()) << '\n';
    std::cout << "Type 4: " << n_type4_relays << ' ' << 3*short_edges_.size() << '\n';

    nodes_ = terminals_ + relays_;
    return new RelaysMSTGraph(nodes_, graph_node_type_, r_c_, theta_c_, communication_edges_, n_total_nodes_omni_);
}

std::vector<Edge*> LongEdgeFirstRelaysAlg::connectTerminalWithType1Relays(MSTNodeLam* terminal) const {
    std::vector<Edge*> communication_edges;

    std::vector<std::vector<Node*>> type_1_relays_connected_components 
        = findConnectedComponentsType1RelaysSurroundingTerminals(terminal->type_1_relays_);
    for(const std::vector<Node*>& component : type_1_relays_connected_components) {
        std::vector<Edge*> type_1_relays_edges = connectType1RelaysInTheSameComponent(component);
        communication_edges += type_1_relays_edges;
    }

    if (type_1_relays_connected_components.size() == 1) {
        Node* type_1_relay = type_1_relays_connected_components[0][0];
        orientNodeToBisectorCoverNode(terminal, type_1_relay);
        communication_edges.push_back(addCommunicationEdge(terminal, type_1_relay));
    } else {
        std::vector<Node*> leaders{type_1_relays_connected_components[0][0], type_1_relays_connected_components[1][0]};
        orientNodeToCoverNodes(terminal, leaders);
        for(Node* leader : leaders) {
            communication_edges.push_back(addCommunicationEdge(terminal, leader));
        }
    }

    setNodeFixed(terminal);
    return communication_edges;
}

SteinerizeShortEdgeResult_LEF LongEdgeFirstRelaysAlg::steinerizeShortEdgeWithOrientation(
    MSTNodeLam* terminal1, MSTNodeLam* terminal2, const std::vector<Point2D>& type_4_relays_pos,
    GraphNodeType graph_node_type
) const {
    SteinerizeShortEdgeResult_LEF result;
    std::vector<MSTNodeLam*> all_nodes;
    all_nodes.push_back(terminal1);
    for(const Point2D& relay_pos : type_4_relays_pos) {
        Node* relay_node = createRelayNode(relay_pos, RELAY_DD_NODE_TYPE_4, graph_node_type, r_c_, theta_c_);
        result.type_4_relays.push_back(relay_node);
        all_nodes.push_back((MSTNodeLam*) relay_node);
    }
    all_nodes.push_back(terminal2);
    for(int i = 1; i < all_nodes.size() - 1; ++i) {
        MSTNodeLam* cur_node = all_nodes[i];
        MSTNodeLam* prev_node = all_nodes[i - 1];
        MSTNodeLam* next_node = all_nodes[i + 1];
        orientNodeToCoverNodes(cur_node, {prev_node, next_node});
        result.communication_edges.push_back(addCommunicationEdge(prev_node, cur_node));
        setNodeFixed(cur_node);
    }

    Node* last_relay_node = result.type_4_relays.back();
    if (terminal2->free_) {
        orientNodeToBisectorCoverNode(terminal2, last_relay_node);
        setNodeFixed(terminal2);
    }
    result.communication_edges.push_back(addCommunicationEdge(terminal2, last_relay_node));

    return result;
}

SteinerizeShortEdgeResult_LEF LongEdgeFirstRelaysAlg::steinerizeShortEdge(
    Edge* short_edge, GraphNodeType graph_node_type
) const {
    SteinerizeShortEdgeResult_LEF result;
    MSTNodeLam* terminal1 = (MSTNodeLam*) short_edge->getEndpoint1();
    MSTNodeLam* terminal2 = (MSTNodeLam*) short_edge->getEndpoint2();

    if (terminal1->free_ && terminal2->free_) {
        orientNodeToBisectorCoverNode(terminal1, terminal2);
        orientNodeToBisectorCoverNode(terminal2, terminal1);
        setNodeFixed(terminal1);
        setNodeFixed(terminal2);
        result.communication_edges.push_back(addCommunicationEdge(terminal1, terminal2));
        return result;
    }

    if (terminal1->free_) std::swap(terminal1, terminal2);

    std::vector<Point2D> type_4_relays_pos;
    if (terminal2->free_) {
        type_4_relays_pos = calculateShortEdgeRelaysPos_OneFree(
            (Sector*) terminal1->getCommunicationAntenna(), (Sector*) terminal2->getCommunicationAntenna());
    } else {
        type_4_relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(
            (Sector*) terminal1->getCommunicationAntenna(), (Sector*) terminal2->getCommunicationAntenna());
    }

    if (type_4_relays_pos.empty()) {
        if (terminal2->free_) {
            orientNodeToBisectorCoverNode(terminal2, terminal1);
            setNodeFixed(terminal2);
        }
        result.communication_edges.push_back(addCommunicationEdge(terminal1, terminal2));
        return result;
    }
    return steinerizeShortEdgeWithOrientation(terminal1, terminal2, type_4_relays_pos, graph_node_type);
}