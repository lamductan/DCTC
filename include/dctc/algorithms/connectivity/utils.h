#ifndef DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_

#include <vector>
#include "dctc/network_components/nodes/node.h"


std::vector<Node*> findConvexHull(std::vector<Node*> nodes);

//Return a vector of 4 nodes in the order: A, B, C, D. Communication edges: AB, AC, BD
std::vector<Node*> orientFourNodesPI_2CoverPlane(std::vector<Node*> nodes);

bool canNodesCoverPoint2D(const std::vector<Node*>& nodes, const Point2D& point2D);
bool canNodesCoverPlane(const std::vector<Node*>& nodes);

#endif //DCTC_ALGORITHMS_CONNECTIVITY_UTILS_H_