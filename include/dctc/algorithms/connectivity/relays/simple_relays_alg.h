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
private:
    GraphNodeType graph_node_type_ = MST_GRAPH_NODE;
protected:
    MSTGraph* MST_graph_;
    std::vector<Edge*> MST_edges_;
    long double r_c_;
    long double theta_c_;
    int n_total_nodes_omni_;
    std::vector<Node*> terminals_;
    std::vector<Node*> relays_;
    std::vector<Node*> nodes_;
    std::vector<Edge*> communication_edges_;
    int n_long_or_medium_edges_;
    int n_short_edges_;

    virtual void init(long double r_c, long double theta_c);
    virtual void connectTerminalsWithType1Relays(
        Edge* edge, const std::vector<Node*>& type_1_relays, std::vector<Edge*>& communication_edges) const;
    virtual void connectType1Type2Relays(
        const std::vector<Node*>& type_1_relays, const std::vector<Node*>& type_2_relays,
        std::vector<Edge*>& communication_edges) const;
    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongOrMediumEdgesWithOrientation(
        Edge* edge, const std::pair<Point2D, Point2D>& type_1_relays_pos,
        const std::vector<Point2D>& type_2_relays_pos, GraphNodeType graph_node_type=MST_GRAPH_NODE) const;
    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongEdges(
        Edge* long_edge, GraphNodeType graph_node_type=MST_GRAPH_NODE) const;
    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeMediumEdge(
        Edge* medium_edge, GraphNodeType graph_node_type=MST_GRAPH_NODE) const;

    RelaysMSTGraph* statAndReturnRelaysMSTGraph();

public:
    SimpleRelaysAlg();
    SimpleRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);

    virtual RelaysMSTGraph* solve();
    bool isShortEdge(Edge* edge) const;
    bool isLongOrMediumEdge(Edge* edge) const;
    bool isLongEdge(Edge* edge) const;
    bool isMediumEdge(Edge* edge) const;
    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongOrMediumEdge(
        Edge* long_or_medium_edge, GraphNodeType graph_node_type=MST_GRAPH_NODE);

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
    std::vector<Edge*> getCommunicationEdges() const;
    long double getBeta() const;
    virtual ~SimpleRelaysAlg();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_SIMPLE_RELAYS_RELAYS_ALG_H_