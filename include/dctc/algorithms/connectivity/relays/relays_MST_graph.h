#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_

#include "dctc/network_components/graphs/MST_graph.h"


class RelaysMSTGraph : public MSTGraph {
protected:
    long double r_c_;
    long double theta_c_;
    int n_total_nodes_omni_;
    std::vector<Node*> terminals_;
    std::vector<Node*> relays_;
    long double beta_;

public:
    RelaysMSTGraph(const std::vector<Node*>& nodes, GraphNodeType graph_node_type, long double r_c, long double theta_c,
                   std::vector<Edge*>& communication_edges, int n_total_nodes_omni);
    GraphNodeType getGraphNodeType() const;
    long double getThetaC() const;
    long double getRC() const;
    int getNumTerminals() const;
    int getNumRelays() const;
    int getNumTotalNodes() const;
    int getNumTotalNodesOmni() const;
    std::vector<Node*> getTerminals() const;
    std::vector<Node*> getRelayNodes() const;
    std::vector<Node*> getAllNodes() const;
    long double getBeta() const;
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_MST_GRAPH_H_