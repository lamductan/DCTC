#ifndef DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_

#include "geometric_primitives/geom2D.h"

#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/nodes/node_factory.h"
#include "dctc/network_components/edges/edge.h"


std::pair<Point2D, Point2D> calculateRelaysType1Positions(const Segment2D& segment2D, long double r_c);
std::vector<Point2D> calculateRelaysType2Positions_LongEdge(
    const Segment2D& segment2D, const std::pair<Point2D, Point2D>& relays_type_1_pos, long double r_c); 
std::vector<Point2D> calculateShortEdgeRelaysPos_TwoNonFree(const Segment2D& segment2D); 

Node* createRelayNode(const Point2D& pos, NodeType node_type, GraphNodeType graph_node_type,
                      long double r_c, long double theta_c);
Node* createRelayNode(const Point2D& pos, NodeType node_type, GraphNodeType graph_node_type,
                      long double r_s, long double theta_s, long double r_c, long double theta_c);
long double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point);
long double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points);
long double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover);
long double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover);
Edge* addCommunicationEdge(Node* a1, Node* b1);

//Find the point on the ray GA with distance r to G.
Point2D findPoint2DWithDistanceFromAPointKnowingAPointInBetween(const Point2D& G, const Point2D& A, long double r);
std::pair<Point2D, Point2D> findPointsToFormIsoscelesRightTriangle(const Point2D& A, const Point2D& B);
std::vector<Point2D> calculateShortEdgeRelaysPos_TwoNonFree_general(Sector* sA, Sector* sB);
std::vector<Point2D> calculateShortEdgeRelaysPos_OneFree(Sector* sA_fixed, Sector* sB_free, Ray2D* Ax=nullptr);

#endif //DCTC_ALGORITHMS_CONNECTIVITY_RELAYS_RELAYS_UTILS_H_