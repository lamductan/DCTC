#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_

#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"


class LongEdgeFirstRelaysAlg : public SimpleRelaysAlg {
public:
    LongEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c);
    virtual RelaysMSTGraph* solve();
}; 

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_LONG_EDGE_FIRST_ALG_H_