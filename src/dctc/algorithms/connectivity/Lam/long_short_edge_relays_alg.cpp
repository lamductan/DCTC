#include <cassert>
#include <unordered_map>

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
            if (node_i->canCoverOtherNodeByCommunicationAntenna(node_j)
                    && node_j->canCoverOtherNodeByCommunicationAntenna(node_i)) {
                communication_edges.push_back(addCommunicationEdge(node_i, node_j));
            }
        }
    }
    return communication_edges;
}

LongShortEdgeRelaysAlg::~LongShortEdgeRelaysAlg() {
    delete MST_graph_;
}