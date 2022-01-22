#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_

#include "geometric_primitives/geom2D.h"
#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"

std::pair<Point2D, Point2D> getRelaysType1Positions(const Segment2D& segment2D, double r_c);
std::vector<Point2D> getRelaysType2Positions(
    const Segment2D& segment2D, const std::pair<Point2D, Point2D>& relays_type_1_pos, double r_c); 

double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point);
double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points);
double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover);
double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover);
Edge* addCommunicationEdge(Node* a1, Node* b1);

#endif //DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_