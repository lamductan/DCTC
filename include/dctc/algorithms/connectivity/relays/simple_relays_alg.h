#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_SIMPLE_RELAYS_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_SIMPLE_RELAYS_ALG_H_

#include <vector>
#include "dctc/algorithms/connectivity/relays/relays_MST_graph.h"


struct SteinerizeLongOrMediumEdgeResult_LEF {
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
    int n_total_nodes_omni_;
    std::vector<Node*> terminals_;
    std::vector<Node*> relays_;
    std::vector<Node*> nodes_;
    std::vector<Edge*> communication_edges_;

public:
    SimpleRelaysAlg(MSTGraph* MST_graph, double r_c, double theta_c);

    virtual RelaysMSTGraph* solve();
    bool isShortEdge(Edge* edge) const;
    bool isLongOrMediumEdge(Edge* edge) const;
    bool isLongEdge(Edge* edge) const;
    bool isMediumEdge(Edge* edge) const;
    void connectTerminalsWithType1Type2Relays(
        Edge* edge, const std::vector<Node*>& type_1_relays, const std::vector<Node*>& type_2_relays,
        std::vector<Edge*>& communication_edges) const;
    SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongOrMediumEdges(
        Edge* edge, const std::pair<Point2D, Point2D>& type_1_relays_pos,
        const std::vector<Point2D>& type_2_relays_pos) const;
    SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongEdges(Edge* long_edge) const;
    SteinerizeLongOrMediumEdgeResult_LEF steinerizeMediumEdge(Edge* medium_edge) const;

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