#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_

#include "dctc/network_components/graphs/MST_graph.h"


class RelaysMSTGraph : public MSTGraph {
protected:
    double r_c_;
    double theta_c_;
    int n_total_nodes_omni_;
    std::vector<Node*> terminals_;
    std::vector<Node*> relays_;
    double beta_;

public:
    RelaysMSTGraph(const std::vector<Node*>& nodes, GraphNodeType graph_node_type, double r_c, double theta_c,
                   std::vector<Edge*>& communication_edges, int n_total_nodes_omni);
    GraphNodeType getGraphNodeType() const;
    double getThetaC() const;
    double getRC() const;
    int getNumTerminals() const;
    int getNumRelays() const;
    int getNumTotalNodes() const;
    int getNumTotalNodesOmni() const;
    std::vector<Node*> getTerminals() const;
    std::vector<Node*> getRelayNodes() const;
    std::vector<Node*> getAllNodes() const;
    double getBeta() const;
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_