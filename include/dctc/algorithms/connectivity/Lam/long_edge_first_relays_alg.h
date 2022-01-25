#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_

#include "dctc/algorithms/connectivity/Lam/long_short_edge_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/MST_node_Lam.h"


struct SteinerizeShortEdgeResult_LEF {
    std::vector<Node*> type_4_relays;
    std::vector<Edge*> communication_edges;
};


class LongEdgeFirstRelaysAlg : public LongShortEdgeRelaysAlg {
protected:
    std::vector<Edge*> connectTerminalWithType1Relays(MSTNodeLam* terminal) const;
    SteinerizeShortEdgeResult_LEF steinerizeShortEdge(Edge* short_edge) const;
public:
    LongEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_