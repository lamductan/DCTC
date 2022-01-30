#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_

#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/MST_node_Lam.h"


struct SteinerizeShortEdgeResult {
    std::vector<Node*> type_4_relays;
    std::vector<Edge*> communication_edges;
};

class LongShortEdgeRelaysAlg : public SimpleRelaysAlg {
protected:
    GraphNodeType graph_node_type_ = MST_GRAPH_NODE_LAM;

    std::vector<Edge*> long_or_medium_edges_;
    std::vector<Edge*> short_edges_;

    virtual void init(long double r_c, long double theta_c);

    static void setNodeFixed(Node* node);

    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongOrMediumEdge(
        Edge* long_or_medium_edge, GraphNodeType graph_node_type);

    static std::vector<std::vector<Node*>> findConnectedComponentsType1RelaysSurroundingTerminals(
        const std::vector<Node*>& type_1_relays);
    std::vector<Edge*> connectType1RelaysInTheSameComponent(const std::vector<Node*>& component) const;

    SteinerizeShortEdgeResult steinerizeShortEdgeWithOrientation(
        MSTNodeLam* terminal1, MSTNodeLam* terminal2, const std::vector<Point2D>& type_4_relays_pos,
        GraphNodeType graph_node_type) const;

public:
    LongShortEdgeRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve() = 0;
    virtual ~LongShortEdgeRelaysAlg();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_