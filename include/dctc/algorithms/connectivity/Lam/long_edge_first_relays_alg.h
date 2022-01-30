#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_

#include "dctc/algorithms/connectivity/Lam/long_short_edge_relays_alg.h"


class LongEdgeFirstRelaysAlg : public LongShortEdgeRelaysAlg {
protected:
    std::vector<Edge*> connectTerminalWithType1Relays(MSTNodeLam* terminal) const;
    SteinerizeShortEdgeResult steinerizeShortEdge(Edge* short_edge, GraphNodeType graph_node_type) const;

public:
    LongEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_