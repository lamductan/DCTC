#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_

#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"


class LongShortEdgeRelaysAlg : public SimpleRelaysAlg {
protected:
    GraphNodeType graph_node_type_ = MST_GRAPH_NODE_LAM;

    std::vector<Edge*> long_or_medium_edges_;
    std::vector<Edge*> short_edges_;

    virtual void init(long double r_c, long double theta_c);
    virtual SteinerizeLongOrMediumEdgeResult_LEF steinerizeLongOrMediumEdge(
        Edge* long_or_medium_edge, GraphNodeType graph_node_type);
    static std::vector<std::vector<Node*>> findConnectedComponentsType1RelaysSurroundingTerminals(
        const std::vector<Node*>& type_1_relays);
    std::vector<Edge*> connectType1RelaysInTheSameComponent(const std::vector<Node*>& component) const;

    static void setNodeFixed(Node* node);

public:
    LongShortEdgeRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve() = 0;
    virtual ~LongShortEdgeRelaysAlg();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_SHORT_EDGE_ALG_H_