#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_SIMPLE_RELAYS_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_SIMPLE_RELAYS_ALG_H_

#include <vector>
#include "dctc/algorithms/connectivity/relays/relays_MST_graph.h"


struct SteinerizeLongEdgeResult_LEF {
    std::vector<Node*> type_1_relays;
    std::vector<Node*> type_2_relays;
    std::vector<Edge*> communication_edges;
};

class SimpleRelaysAlg {
protected:
    MSTGraph* MST_graph_;
    std::vector<Edge*> MST_edges_;
    GraphNodeType graph_node_type_;
    double r_c_;
    double theta_c_;
    int n_total_nodes_omni_ = 0;
    std::vector<Node*> terminals_;
    std::vector<Node*> relays_;
    std::vector<Node*> nodes_;
    std::vector<Edge*> communication_edges_;

    bool isShortEdge(Edge* edge) const;
    bool isLongEdge(Edge* edge) const;
    bool isMediumEdge(Edge* edge) const;

public:
    SimpleRelaysAlg(MSTGraph* MST_graph, double r_c, double theta_c);
    virtual RelaysMSTGraph* solve();
    SteinerizeLongEdgeResult_LEF steinerizeMediumEdge(Edge* medium_edge) const;
    SteinerizeLongEdgeResult_LEF steinerizeLongEdgesUpAndDown(Edge* long_edge) const;
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
    std::vector<Edge*> getCommunicationEdges() const;
    double getBeta() const;
    virtual ~SimpleRelaysAlg();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_SIMPLE_RELAYS_RELAYS_ALG_H_