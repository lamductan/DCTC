#include <cassert>
#include <unordered_map>

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
    for(Node* terminal_ : terminals_) {
        MSTNodeLam* terminal = (MSTNodeLam*) terminal_;
        if (terminal->type_1_relays_.empty()) continue;
        std::vector<Edge*> new_communication_edges = connectTerminalWithType1Relays(terminal);
        communication_edges_ += new_communication_edges;
    }

    // Step 3: Connect terminals on short edges
    std::cout << "Step 3: Connect terminals on short edges\n";
    for(Edge* short_edge : short_edges_) {
        SteinerizeShortEdgeResult steinerize_short_edge_result = steinerizeShortEdge(short_edge, graph_node_type_);
        relays_ += steinerize_short_edge_result.type_4_relays;
        communication_edges_ += steinerize_short_edge_result.communication_edges;
    }

    return statAndReturnRelaysMSTGraph();
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

SteinerizeShortEdgeResult LongEdgeFirstRelaysAlg::steinerizeShortEdge(
    Edge* short_edge, GraphNodeType graph_node_type
) const {
    SteinerizeShortEdgeResult result;
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