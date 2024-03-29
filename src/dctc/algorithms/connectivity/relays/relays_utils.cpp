#include <cassert>
#include <algorithm>
#include <iostream>

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
    Point2D relay_type_1_pos_1 = relays_type_1_pos.first;
    Point2D relay_type_1_pos_2 = relays_type_1_pos.second;
    if (computeEuclidDistance(relay_type_1_pos_1, relay_type_1_pos_2) < r_c*0.2) {
        Segment2D s1 = Segment2D(segment2D.getEndpoint1(), relay_type_1_pos_1);
        Segment2D s2 = Segment2D(segment2D.getEndpoint2(), relay_type_1_pos_1);
        return {s1.getMidPoint(), s2.getMidPoint()};
    }
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

Node* createRelayNode(const Point2D& pos, NodeType node_type, GraphNodeType graph_node_type,
                      long double r_c, long double theta_c) 
{
    return createRelayNode(pos, node_type, graph_node_type, r_c, theta_c, r_c, theta_c);
}

Node* createRelayNode(const Point2D& pos, NodeType node_type, GraphNodeType graph_node_type,
                      long double r_s, long double theta_s, long double r_c, long double theta_c)
{
    Node* node = new DDNode(pos.getX(), pos.getY(), node_type, r_s, r_c, theta_s, theta_c);
    return NodeFactory::createNode(node, graph_node_type, false);
}

long double orientNodeToBisectorCoverPoint2D(Node* node, const Point2D& point) {
    return dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoint2D(point, false);
}

long double orientNodeToCoverPoints2D(Node* node, const std::vector<Point2D>& points) {
    long double result = dynamic_cast<Sector*>(node->getCommunicationAntenna())->orientToCoverPoints2D(points, false);
    if (result == -1) {
        std::cout << "terminal = " << *node << '\n';
        print_vector<Point2D>(points, '\n');
    }
    assert(result != -1);
    return result;
}

long double orientNodeToBisectorCoverNode(Node* node_to_orient, Node* node_to_cover) {
    return orientNodeToBisectorCoverPoint2D(node_to_orient, node_to_cover->getPoint2D());
}

long double orientNodeToCoverNodes(Node* node_to_orient, const std::vector<Node*>& nodes_to_cover) {
    if (nodes_to_cover.size() == 1)
        return orientNodeToBisectorCoverNode(node_to_orient, nodes_to_cover[0]);
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

Point2D findPoint2DWithDistanceFromAPointKnowingAPointInBetween(const Point2D& G, const Point2D& A, long double r) {
    Ray2D ray_GA = Ray2D::fromTwoPoints(G, A);
    Circle C(G, r);
    std::vector<Point2D> intersections = C.findIntersectionsWithRay2D(ray_GA);
    Point2D H = intersections[0];
    if (H == G) H = intersections[1];
    assert(fabs(computeEuclidDistance(G, H) - r) <= EPSILON);
    return H;
}

std::pair<Point2D, Point2D> findPointsToFormIsoscelesRightTriangle(const Point2D& A, const Point2D& B) {
    Point2D M = (A + B) / 2;
    Point2D C1 = rotate(A, M, PI_2);
    Point2D C2 = M*2 - C1;
    return {C1, C2};
}

std::vector<Point2D> calculateShortEdgeRelaysPos_TwoNonFree_general(Sector* sA, Sector* sB) {
    Point2D A = sA->getCenter();
    Point2D B = sB->getCenter(); 
    Segment2D AB = Segment2D(A, B);
    Ray2D ray_AB = Ray2D::fromTwoPoints(A, B);
    Ray2D ray_BA = Ray2D::fromTwoPoints(B, A);
    long double length_AB = AB.length();
    long double r_A = sA->getRadius();
    long double r_B = sB->getRadius();
    long double r = std::min(r_A, r_B);
    assert(AB.length() <= r + EPSILON);

    //std::cout << A << ' ' << sA->getOrientationAngle() << '\n';
    //std::cout << B << ' ' << sB->getOrientationAngle() << '\n';

    bool sA_contains_B = sA->containsPoint2D(B);
    bool sB_contains_A = sB->containsPoint2D(A);

    if (sA_contains_B && sB_contains_A)
        return std::vector<Point2D>();

    Segment2D A_A1 = sA->getBoundarySegment1();
    Segment2D A_A2 = sA->getBoundarySegment2();
    Line2D l1 = getLine2DParallelLineAndPassingThroughPoint(A_A1, B);
    if (!sB->arcIntersectsWithLine2D(l1)) {
        l1 = getLine2DParallelLineAndPassingThroughPoint(A_A2, B);
        A_A1 = sA->getBoundarySegment2();
        A_A2 = sA->getBoundarySegment1();
    }

    bool is_l1_coincident_with_AB = (l1.containsPoint2D(A) & l1.containsPoint2D(B));
    bool is_l1_perpendicular_to_AB = isLinePerpendicularLine(l1, AB);

    std::vector<Point2D> res;
    if (!is_l1_coincident_with_AB && !is_l1_perpendicular_to_AB) {
        //std::cout << "normal case\n";
        if (sB_contains_A || (!sB_contains_A && !sA_contains_B)) {
            //std::cout << "l1 = " << l1 << '\n';
            //std::cout << "A_A2 = " << (Line2D) A_A2 << '\n';
            Point2D G = l1.findIntersectionsWithLine2D(A_A2);
            //std::cout << "G = " << G << '\n';
            bool sA_contains_G = sA->containsPoint2D(G);
            bool sB_contains_G = sB->containsPoint2D(G);
            if (sA_contains_G && sB_contains_G) {
                res.push_back(G);
                //return res;
            } else {
                if (sB_contains_G) {
                    Point2D H = findPoint2DWithDistanceFromAPointKnowingAPointInBetween(G, A, r);
                    res.push_back(H);
                    res.push_back(G);
                    //return res;
                } else if (sA_contains_G) {
                    Point2D H = findPoint2DWithDistanceFromAPointKnowingAPointInBetween(G, B, r);
                    res.push_back(G);
                    res.push_back(H);
                    //return res;
                } else {
                    Point2D H = findPoint2DWithDistanceFromAPointKnowingAPointInBetween(G, A, r);
                    res.push_back(H);
                    res.push_back(G);
                    Point2D I = findPoint2DWithDistanceFromAPointKnowingAPointInBetween(G, B, r);
                    res.push_back(I);
                    //return res;
                } 
            } 
        } else { //(sA_contains_B)
            std::vector<Point2D> res1 = calculateShortEdgeRelaysPos_TwoNonFree_general(sB, sA);
            std::reverse(res1.begin(), res1.end());
            return res1;
        }
    } else if (is_l1_coincident_with_AB) {
        //std::cout << "l1_coincidient_with_AB\n";
        if (sB_contains_A) {
            Point2D G = getPointOnSegmentAtDistanceFromEndpoint1(A_A1, r*0.2);
            res.push_back(G);
            std::pair<Point2D, Point2D> Hs = findPointsToFormIsoscelesRightTriangle(G, B);
            Point2D H = Hs.first;
            if (!sB->containsPoint2D(H)) {
                H = Hs.second;
            }
            res.push_back(H);
            //return res;
        } else if (sA_contains_B) {
            Point2D G = getPointOnRayAtDistance(ray_AB, length_AB + r*0.2);
            std::pair<Point2D, Point2D> Hs = findPointsToFormIsoscelesRightTriangle(G, A);
            Point2D H = Hs.first;
            if (!sA->containsPoint2D(H)) {
                H = Hs.second;
            }
            res.push_back(H);
            res.push_back(G);
            //return res;

        } else {
            Point2D G = getPointOnSegmentAtDistanceFromEndpoint1(A_A1, r*0.2);
            res.push_back(G);
            Point2D I = getPointOnRayAtDistance(ray_AB, length_AB + r*0.2);
            std::pair<Point2D, Point2D> Hs = findPointsToFormIsoscelesRightTriangle(G, I);
            Point2D H = Hs.first;
            if (!sB->containsPoint2D(H)) {
                H = Hs.second;
            }
            res.push_back(H);
            res.push_back(I);
            //return res;
        }
    } else {
        //std::cout << "l1_perpendicular_to_AB\n";
        Point2D G = getPointOnSegmentAtDistanceFromEndpoint1(A_A1, r*0.2);
        res.push_back(G);
        Point2D H = G + B - A;
        res.push_back(H);
        if (sB->containsPoint2D(H)) {
            //return res;
        } else {
            Point2D I = findPoint2DWithDistanceFromAPointKnowingAPointInBetween(H, B, r);
            res.push_back(I);
            //return res;
        } 
    }

    //print_vector<Point2D>(res, '\n');
    assert(res.size() <= 3);
    assert(sA->containsPoint2D(res[0]));
    assert(sB->containsPoint2D(res.back()));
    std::vector<Point2D> points;
    points.push_back(A);
    points += res;
    points.push_back(B);
    for(int i = 1; i < points.size() - 1; ++i) {
        if (!(computeGeometricAngle(points[i - 1], points[i], points[i + 1]) <= PI_2 + EPSILON*2.5)) {
            std::cout << "BUG at " << i << '\n';
            long double angle = computeGeometricAngle(points[i - 1], points[i], points[i + 1]);
            std::cout << fabs(angle - PI_2) - EPSILON*2.5 << '\n';
            std::cout << i << ' ' << computeGeometricAngle(points[i - 1], points[i], points[i + 1]) << '\n';
            print_vector<Point2D>(points, '\n');
            std::cout << sA->getOrientationAngle() << ' ' << sB->getOrientationAngle() << '\n';
        }
        assert(computeGeometricAngle(points[i - 1], points[i], points[i + 1]) <= PI_2 + EPSILON*2.5);
        assert(computeEuclidDistance(points[i - 1], points[i]) <= r + EPSILON);
        assert(computeEuclidDistance(points[i], points[i + 1]) <= r + EPSILON);
    }
    return res;
}

std::vector<Point2D> calculateShortEdgeRelaysPos_OneFree(Sector* sA_fixed, Sector* sB_free, Ray2D* Ax) {
    Point2D A = sA_fixed->getCenter();
    Point2D B = sB_free->getCenter(); 
    Segment2D AB = Segment2D(A, B);
    long double r_A = sA_fixed->getRadius();
    long double r_B = sB_free->getRadius();
    long double r = std::min(r_A, r_B);
    assert(AB.length() <= r + EPSILON);

    std::vector<Point2D> res = std::vector<Point2D>();
    bool delete_Ax_later = false;
    if (Ax == nullptr) {
        if (sA_fixed->containsPoint2D(B)) return res;
        Ray2D ray = Ray2D::fromTwoPoints(A, sA_fixed->getEndpoint1());
        Ax = new Ray2D(ray);
        delete_Ax_later = true;
    }

    if (Ax->containsPoint2D(B)) return res;

    Point2D C = getOrthogonalProjectionOfPoint2DOnLine(B, *Ax);
    if (!C.isPointInfinity() && Ax->containsPoint2D(C) && computeEuclidDistance(C, A) >= r*0.2) {
        res.push_back(C);
        //return res;
    } else {
        Point2D D = getPointOnRayAtDistance(*Ax, r*0.2);
        assert(!D.isPointInfinity());
        std::pair<Point2D, Point2D> Es = findPointsToFormIsoscelesRightTriangle(D, B);
        Point2D E = Es.first;
        if (computeGeometricAngle(A, D, E) > PI_2 + EPSILON) E = Es.second;
        res.push_back(D);
        res.push_back(E);
        //return res;
    }

    assert(res.size() <= 2);
    assert(sA_fixed->containsPoint2D(res[0]));
    std::vector<Point2D> points;
    points.push_back(A);
    points += res;
    points.push_back(B);
    for(int i = 1; i < points.size() - 1; ++i) {
        assert(computeGeometricAngle(points[i - 1], points[i], points[i + 1]) <= PI_2 + EPSILON*2.5);
        assert(computeEuclidDistance(points[i - 1], points[i]) <= r + EPSILON);
        assert(computeEuclidDistance(points[i], points[i + 1]) <= r + EPSILON);
    }
    if (delete_Ax_later) delete Ax;
    return res;
}