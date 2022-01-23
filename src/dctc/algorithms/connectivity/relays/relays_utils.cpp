#include <cassert>

#include "dctc/network_components/nodes/DD_node.h"
#include "dctc/network_components/nodes/MST_node.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"


std::pair<Point2D, Point2D> calculateRelaysType1Positions(const Segment2D& segment2D, long double r_c) {
    long double half_r_c = r_c/2;
    Point2D relay_type_1_1 = getPointOnSegmentAtDistanceFromEndpoint1(segment2D, half_r_c);
    Point2D relay_type_1_2 = getPointOnSegmentAtDistanceFromEndpoint2(segment2D, half_r_c);
    return {relay_type_1_1, relay_type_1_2};
}

std::vector<Point2D> calculateRelaysType2Positions_LongEdge(
    const Segment2D& segment2D, const std::pair<Point2D, Point2D>& relays_type_1_pos, long double r_c
) {
    int d = ceil(segment2D.length() / r_c);
    assert(d > 1);
    Point2D D0 = rotate(segment2D.getEndpoint1(), relays_type_1_pos.first, PI/4);
    Point2D U0 = relays_type_1_pos.first*2 - D0;
    Vector2D v = Vector2D(relays_type_1_pos.first, relays_type_1_pos.second) / (d - 1);

    std::vector<Point2D> type_2_relay_pos{D0, U0};
    for(int i = 0; i < d - 1; ++i) {
        int m = type_2_relay_pos.size();
        Point2D D = type_2_relay_pos[m - 2];
        Point2D U = type_2_relay_pos[m - 1];
        type_2_relay_pos.push_back(D + v);
        type_2_relay_pos.push_back(U + v);
    }
    return type_2_relay_pos;
}

std::vector<Point2D> calculateShortEdgeRelaysPos_TwoNonFree(const Segment2D& segment2D) {
    long double r = segment2D.length();
    long double r1 = r*0.2L;
    Point2D P0_prime = getPointOnSegmentAtDistanceFromEndpoint1(segment2D, r1);
    Point2D P0 = segment2D.getEndpoint1()*2 - P0_prime;
    Point2D P2_prime = getPointOnSegmentAtDistanceFromEndpoint2(segment2D, r1);
    Point2D P2 = segment2D.getEndpoint2()*2 - P2_prime;
    Point2D M = segment2D.getMidPoint();
    Point2D P1 = rotate(P0, M, -PI_2);
    return {P0, P1, P2};
}

Node* createRelayNode(const Point2D& pos, NodeType node_type, long double r_c, long double theta_c) {
    return createRelayNode(pos, node_type, r_c, theta_c, r_c, theta_c);
}

Node* createRelayNode(const Point2D& pos, NodeType node_type, long double r_s, long double theta_s, long double r_c, long double theta_c) {
    Node* node = new DDNode(pos.getX(), pos.getY(), node_type, r_s, r_c, theta_s, theta_c);
    return new MSTNode(node, false);
}

long double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point) {
    return dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoint2D(point, false);
}

long double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points) {
    return dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoints2D(points, false);
}

long double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover) {
    return orientNodeToBisectorCoverPoint2D(node_to_cover, node_to_cover->getPoint2D());
}

long double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover) {
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