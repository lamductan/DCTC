#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_

#include "dctc/algorithms/connectivity/Lam/long_short_edge_relays_alg.h"


class ShortEdgeFirstRelaysAlg : public LongShortEdgeRelaysAlg {
private:
    static Point2D findType4RelayPos(Node* terminal, Node* type_1_relay);
protected:
    void steinerizeAndOrientShortSubtree(MSTNodeLam* terminal);
    SteinerizeShortEdgeResult steinerizeShortEdge(Edge* short_edge, GraphNodeType graph_node_type) const;
    void dfsShortSubtree(MSTNodeLam* terminal, MSTNodeLam* parent, Edge* edge_to_parent);
    void connectTerminalWithType1Relays(MSTNodeLam* terminal);
public:
    ShortEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_