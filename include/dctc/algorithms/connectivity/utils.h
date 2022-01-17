#ifndef DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_

#include <vector>
#include "dctc/network_components/nodes/node.h"


std::vector<Node*> findConvexHull(std::vector<Node*> nodes);
void orientFourNodesPI_2CoverPlane(std::vector<Node*> nodes);

#endif //DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_