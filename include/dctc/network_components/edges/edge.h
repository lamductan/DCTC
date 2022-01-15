#ifndef DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_
#define DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_

#include "geometric_primitives/geom2D.h"


class Edge {
private:
    int id;
    Segment2D segment2D;

public:
    Edge();
};

#endif //DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_