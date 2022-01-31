#include <cassert>

#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/Lam/long_short_edge_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/MST_node_Lam.h"


void LongShortEdgeRelaysAlg::init(long double r_c, long double theta_c) {
    SimpleRelaysAlg::init(r_c, theta_c);
    for(Edge* MST_edge : MST_edges_) {
        Node* endpoint1 = MST_edge->getEndpoint1();
        Node* endpoint2 = MST_edge->getEndpoint2();
        if (isLongOrMediumEdge(MST_edge)) {
            long_or_medium_edges_.push_back(MST_edge);
            dynamic_cast<MSTNodeLam*>(endpoint1)->long_or_medium_edge_adj_nodes_.push_back(endpoint2);
            dynamic_cast<MSTNodeLam*>(endpoint2)->long_or_medium_edge_adj_nodes_.push_back(endpoint1);
        } else {
            short_edges_.push_back(MST_edge);
            dynamic_cast<MSTNodeLam*>(endpoint1)->short_edge_adj_nodes_.push_back(endpoint2);
            dynamic_cast<MSTNodeLam*>(endpoint2)->short_edge_adj_nodes_.push_back(endpoint1);
        }
    }
    n_long_or_medium_edges_ = long_or_medium_edges_.size();
    n_short_edges_ = short_edges_.size();
    std::cout << "n_long_or_medium_edges = " << n_long_or_medium_edges_ <<", n_short_edges = " << n_short_edges_ << '\n';
}

LongShortEdgeRelaysAlg::LongShortEdgeRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c) {
    MST_graph_ = MST_graph->deepCopy(graph_node_type_);
    init(r_c, theta_c);
}

void LongShortEdgeRelaysAlg::setNodeFixed(Node* node) {
    dynamic_cast<MSTNodeLam*>(node)->free_ = false;
}

SteinerizeLongOrMediumEdgeResult_LEF LongShortEdgeRelaysAlg::steinerizeLongOrMediumEdge(
    Edge* long_or_medium_edge, GraphNodeType graph_node_type
) {
    SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result 
        = SimpleRelaysAlg::steinerizeLongOrMediumEdge(long_or_medium_edge, graph_node_type);
    MSTNodeLam* endpoint1 = (MSTNodeLam*) long_or_medium_edge->getEndpoint1();
    endpoint1->type_1_relays_.push_back(steinerize_long_or_medium_edge_result.type_1_relays[0]);
    MSTNodeLam* endpoint2 = (MSTNodeLam*) long_or_medium_edge->getEndpoint2();
    endpoint2->type_1_relays_.push_back(steinerize_long_or_medium_edge_result.type_1_relays[1]);

    for(Node* relay_node : steinerize_long_or_medium_edge_result.type_1_relays) {
        setNodeFixed(relay_node);
    }
    for(Node* relay_node : steinerize_long_or_medium_edge_result.type_2_relays) {
        setNodeFixed(relay_node);
    }
    return steinerize_long_or_medium_edge_result;
}

void dfs(Node* u, const std::vector<Node*> type_1_relays, std::unordered_map<Node*, bool>& vis,
         std::vector<Node*>& component
) {
    if (vis[u]) return;
    vis[u] = true;
    component.push_back(u);
    for(Node* v : type_1_relays) {
        if (vis[v]) continue;
        if (u->canCoverOtherNodeByCommunicationAntenna(v) && v->canCoverOtherNodeByCommunicationAntenna(u)) {
            dfs(v, type_1_relays, vis, component);
        }
    }
}

std::vector<std::vector<Node*>> LongShortEdgeRelaysAlg::findConnectedComponentsType1RelaysSurroundingTerminals(
    const std::vector<Node*>& type_1_relays
) {
    int n = type_1_relays.size();
    std::vector<std::vector<Node*>> connected_components;
    if (n <= 1) {
        connected_components.push_back(type_1_relays);
        return connected_components;
    }

    std::unordered_map<Node*, bool> vis;
    for(Node* type_1_relay : type_1_relays) {
        if (!vis[type_1_relay]) {
            std::vector<Node*> component;
            dfs(type_1_relay, type_1_relays, vis, component);
            connected_components.push_back(component);
        }
    }
    
    assert(connected_components.size() <= 2);
    return connected_components;
}

std::vector<Edge*> LongShortEdgeRelaysAlg::connectType1RelaysInTheSameComponent(
    const std::vector<Node*>& component
) const {
    std::vector<Edge*> communication_edges;
    int n = component.size();
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            Node* node_i = component[i];
            Node* node_j = component[j];
            if (canTwoNodesConnectEachOther(node_i, node_j)) {
                communication_edges.push_back(addCommunicationEdge(node_i, node_j));
            }
        }
    }
    return communication_edges;
}

SteinerizeShortEdgeResult LongShortEdgeRelaysAlg::steinerizeShortEdgeWithOrientation(
    MSTNodeLam* terminal1, MSTNodeLam* terminal2, const std::vector<Point2D>& type_3_relays_pos,
    GraphNodeType graph_node_type
) const {
    SteinerizeShortEdgeResult result;
    std::vector<MSTNodeLam*> all_nodes;
    all_nodes.push_back(terminal1);
    for(const Point2D& relay_pos : type_3_relays_pos) {
        Node* relay_node = createRelayNode(relay_pos, RELAY_DD_NODE_TYPE_3, graph_node_type, r_c_, theta_c_);
        result.type_3_relays.push_back(relay_node);
        all_nodes.push_back((MSTNodeLam*) relay_node);
    }
    all_nodes.push_back(terminal2);
    for(int i = 1; i < all_nodes.size() - 1; ++i) {
        MSTNodeLam* cur_node = all_nodes[i];
        MSTNodeLam* prev_node = all_nodes[i - 1];
        MSTNodeLam* next_node = all_nodes[i + 1];
        long double orientation_angle = orientNodeToCoverNodes(cur_node, {prev_node, next_node});
        assert(orientation_angle != -1);
        if (!cur_node->canCoverOtherNodeByCommunicationAntenna(next_node)) {
            std::cout << "BUG " << __PRETTY_FUNCTION__ << '\n';
            std::cout << "orientation_angle = " << orientation_angle << '\n';
            std::cout << dynamic_cast<Sector*>(
                cur_node->getCommunicationAntenna())->calculateCriterion(next_node->getPoint2D()) << '\n';
            for(Node* node : all_nodes) {
                std::cout << *node->getCommunicationAntenna() << '\n';
            }
            std::cout << "END BUG\n";
        }
        assert(cur_node->canCoverOtherNodeByCommunicationAntenna(prev_node));
        assert(cur_node->canCoverOtherNodeByCommunicationAntenna(next_node));
        result.communication_edges.push_back(addCommunicationEdge(prev_node, cur_node));
        setNodeFixed(cur_node);
    }

    Node* last_relay_node = result.type_3_relays.back();
    if (terminal2->free_) {
        orientNodeToBisectorCoverNode(terminal2, last_relay_node);
        setNodeFixed(terminal2);
    }
    result.communication_edges.push_back(addCommunicationEdge(terminal2, last_relay_node));

    return result;
}

LongShortEdgeRelaysAlg::~LongShortEdgeRelaysAlg() {
    delete MST_graph_;
}