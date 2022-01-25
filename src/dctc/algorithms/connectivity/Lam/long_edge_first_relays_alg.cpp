#include <cassert>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"

RelaysMSTGraph* LongEdgeFirstRelaysAlg::solve() {
    // Step 1: Connect relay nodes on long edges
    for(Edge* long_or_medium_edge : long_or_medium_edges_) {
        SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result = steinerizeLongOrMediumEdge(
            long_or_medium_edge);
    }

    // Step 2: Connect terminals to type-1 relay nodes
    for(Node* terminal : terminals_) {
        std::vector<Edge*> new_communication_edges = connectTerminalWithType1Relays((MSTNodeLam*) terminal);
        communication_edges_ += new_communication_edges;
    }

    // Step 3: Connect terminals on short edges
    for(Edge* short_edge : short_edges_) {
        SteinerizeShortEdgeResult_LEF steinerize_short_edge_result = steinerizeShortEdge(short_edge);
        relays_ += steinerize_short_edge_result.type_4_relays;
        communication_edges_ += steinerize_short_edge_result.communication_edges;
    }
    return new RelaysMSTGraph(nodes_, graph_node_type_, r_c_, theta_c_, communication_edges_, n_total_nodes_omni_);
}

std::vector<Edge*> LongEdgeFirstRelaysAlg::connectTerminalWithType1Relays(MSTNodeLam* terminal) const {
    std::vector<Edge*> communication_edges;
    std::vector<std::vector<Node*>> type_1_relays_connected_components = findConnectedComponentsType1RelaysSurroundingTerminals(
        terminal->type_1_relays_);
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
    return communication_edges;
}

SteinerizeShortEdgeResult_LEF LongEdgeFirstRelaysAlg::steinerizeShortEdge(Edge* short_edge) const {
    SteinerizeShortEdgeResult_LEF result;
    //TODO: implement
    return result;
}