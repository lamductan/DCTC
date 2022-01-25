#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_

#include "dctc/algorithms/connectivity/Lam/long_short_edge_relays_alg.h"


class ShortEdgeFirstRelaysAlg : public LongShortEdgeRelaysAlg {
public:
    ShortEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_SHORT_EDGE_FIRST_ALG_H_