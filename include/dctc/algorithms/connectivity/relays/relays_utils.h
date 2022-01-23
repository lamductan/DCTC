#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_

#include "geometric_primitives/geom2D.h"
#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"

std::pair<Point2D, Point2D> calculateRelaysType1Positions(const Segment2D& segment2D, long double r_c);
std::vector<Point2D> calculateRelaysType2Positions_LongEdge(
    const Segment2D& segment2D, const std::pair<Point2D, Point2D>& relays_type_1_pos, long double r_c); 
std::vector<Point2D> calculateShortEdgeRelaysPos_TwoNonFree(const Segment2D& segment2D); 

Node* createRelayNode(const Point2D& pos, NodeType node_type, long double r_c, long double theta_c);
Node* createRelayNode(const Point2D& pos, NodeType node_type, long double r_s, long double theta_s, long double r_c, long double theta_c);
long double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point);
long double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points);
long double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover);
long double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover);
Edge* addCommunicationEdge(Node* a1, Node* b1);

#endif //DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_