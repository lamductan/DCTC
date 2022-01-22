#include <cassert>

#include "dctc/network_components/nodes/MST_node.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"


std::pair<Point2D, Point2D> getRelaysType1Positions(const Segment2D& segment2D, double r_c) {
    double half_r_c = r_c/2;
    Point2D relay_type_1_1 = getPointOnSegmentAtDistanceFromEndpoint1(segment2D, half_r_c);
    Point2D relay_type_1_2 = getPointOnSegmentAtDistanceFromEndpoint2(segment2D, half_r_c);
    return {relay_type_1_1, relay_type_1_2};
}

std::vector<Point2D> getRelaysType2Positions(
    const Segment2D& segment2D, const std::pair<Point2D, Point2D>& relays_type_1_pos, double r_c
) {
    Point2D U0 = rotate(segment2D.getEndpoint1(), relays_type_1_pos.first, -PI/4);
    Point2D D0 = relays_type_1_pos.first*2 - U0;
    Vector2D v = Vector2D(relays_type_1_pos.first, relays_type_1_pos.second) / r_c;
    
    //TODO: implement
    return std::vector<Point2D>();
}

double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point) {
    return dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoint2D(point, false);
}

double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points) {
    return dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoints2D(points, false);
}

double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover) {
    return orientNodeToBisectorCoverPoint2D(node_to_cover, node_to_cover->getPoint2D());
}

double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover) {
    std::vector<Point2D> points_to_cover;
    for(const Node* node_to_cover : nodes_to_cover) points_to_cover.push_back(node_to_cover->getPoint2D());
    return orientNodeToCoverPoints2D(node_to_orient, points_to_cover);
}

Edge* addCommunicationEdge(Node* a1, Node* b1) {
    MSTNode* a = (MSTNode*) a1;
    MSTNode* b = (MSTNode*) b1;
    Edge* edge = new Edge(a, b);
    a->addCommunicationEdge(edge);
    b->addCommunicationEdge(edge);
    if (!CommunicationChecker::checkAngleOneNode1(a)) std::cout << "failed angle " << *a << ' ' << *b << '\n';
    if (!CommunicationChecker::checkAngleOneNode1(b)) std::cout << "failed angle " << *b << ' ' << *a << '\n';
    assert(CommunicationChecker::checkAngleOneNode1(a));
    assert(CommunicationChecker::checkAngleOneNode1(b));
    return edge;
}