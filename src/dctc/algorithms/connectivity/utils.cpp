#include <cassert>
#include <algorithm>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/utils.h"


std::vector<Node*> orientFourNodesPI_2CoverPlane(std::vector<Node*> nodes) {
    assert(nodes.size() == 4);
    std::vector<Node*> convex_hull = findConvexHull(nodes); 

    Node *nA, *nB, *nC, *nD;
    Point2D pA, pB, pC, pD;
    Sector *sector_A, *sector_B, *sector_C, *sector_D;
    double orientation_angle_A = 0.0;
    double orientation_angle_B = 0.0;

    int m = convex_hull.size();
    if (m < 3) {
        std::sort(nodes.begin(), nodes.end(), 
                  [](Node* a, Node* b) {return a->getPoint2D() < b->getPoint2D();});
        nA = nodes[0], nB = nodes[1];
        nC = nodes[2], nD = nodes[3];
        Point2D pA = nA->getPoint2D(), pB = nB->getPoint2D();
        Point2D pC = nC->getPoint2D(), pD = nD->getPoint2D();
        Point2D pC_1 = rotate(pC, pA, -PI_4); 
        Point2D pC_2 = rotate(pC, pA, PI_4); 
        sector_A = (Sector*) nA->getCommunicationAntenna();
        sector_B = (Sector*) nB->getCommunicationAntenna();
        orientation_angle_A = sector_A->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pC, pC_1, false);
        orientation_angle_B = sector_B->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pC, pC_2, false);
    } 
    else if (m == 3) {
        for(int i = 0; i < 3; ++i) {
            nA = convex_hull[i], nB = convex_hull[(i + 1) % 3], nC = convex_hull[(i + 2) % 3];
            pA = nA->getPoint2D(), pB = nB->getPoint2D(), pC = nC->getPoint2D();
            if (computeGeometricAngle(pC, pA, pB) <= PI_2 && computeGeometricAngle(pC, pB, pA) <= PI_2)
                break;
        }
        sector_A = (Sector*) nA->getCommunicationAntenna();
        sector_B = (Sector*) nB->getCommunicationAntenna();
        orientation_angle_A = sector_A->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pB, pC, false);
        orientation_angle_B = sector_B->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pA, pC, false);
        for(int i = 0; i < 4; ++i) {
            Node* t = nodes[i];
            if (t != nA && t != nB && t != nC) {
                nD = t;
                pD = nD->getPoint2D();
                break;
            }
        }
    }
    else if (m == 4) {
        Point2D pO = Segment2D(convex_hull[0]->getPoint2D(), convex_hull[2]->getPoint2D())
            .findIntersectionsWithSegment2D(Segment2D(convex_hull[1]->getPoint2D(), convex_hull[3]->getPoint2D()));
        assert(!pO.isPointInfinity());
        for(int i = 0; i < 4; ++i) {
            nA = convex_hull[i], nB = convex_hull[(i + 1) % 4];
            nC = convex_hull[(i + 2) % 4], nD = convex_hull[(i + 3) % 4];
            pA = nA->getPoint2D(), pB = nB->getPoint2D();
            pC = nC->getPoint2D(), pD = nD->getPoint2D();
            if (computeGeometricAngle(pO, pA, pB) <= PI_2 && computeGeometricAngle(pO, pB, pA) <= PI_2)
                break;
        }
        sector_A = (Sector*) nA->getCommunicationAntenna();
        sector_B = (Sector*) nB->getCommunicationAntenna();
        orientation_angle_A = sector_A->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pB, pC, false);
        assert(orientation_angle_A != -1);
        orientation_angle_B = sector_B->orientBoundaryPassingThroughPointAndCoverAnotherPoint(pA, pD, false);
        assert(orientation_angle_B != -1);
    }

    sector_C = (Sector*) nC->getCommunicationAntenna();
    sector_D = (Sector*) nD->getCommunicationAntenna();
    sector_C->orientWithAngle(computeOppositeAngle(orientation_angle_A));
    sector_D->orientWithAngle(computeOppositeAngle(orientation_angle_B));
    if (!sector_C->containsPoint2DWithInfRange(sector_D->getCenter())
        && !sector_D->containsPoint2DWithInfRange(sector_C->getCenter()))
    {
        std::swap(nC, nD);
        sector_C = (Sector*) nC->getCommunicationAntenna();
        sector_D = (Sector*) nD->getCommunicationAntenna();
        sector_C->orientWithAngle(computeOppositeAngle(orientation_angle_A));
        sector_D->orientWithAngle(computeOppositeAngle(orientation_angle_B));
    }

    std::vector<Node*> nodes_after_orientation{nA, nB, nC, nD};
    if (!canNodesCoverPlane(nodes_after_orientation)) {
        print_vector_ptr<Node>(nodes_after_orientation, '\n');
    }
    assert(canNodesCoverPlane(nodes_after_orientation));
    return nodes_after_orientation;
}

bool canNodesCoverPoint2D(const std::vector<Node*>& nodes, const Point2D& point2D) {
    for(const Node* node : nodes) 
        if (node->getCommunicationAntenna()->containsPoint2DWithInfRange(point2D)) return true;
    return false;
}

bool canNodesCoverPlane(const std::vector<Node*>& nodes) {
    double min_x = INT_MAX, max_x = INT_MIN;
    double min_y = INT_MAX, max_y = INT_MIN;
    for(const Node* node : nodes) {
        double x = node->getX();
        double y = node->getY();
        if (x < min_x) min_x = x;
        else if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        else if (y > max_y) max_y = y;
    }

    min_x -= 1, max_x += 1;
    min_y -= 1, max_y += 1;
    Point2D A(min_x, max_y), B(max_x, max_y), C(max_x, min_y), D(min_x, min_y);
    Segment2D AB(A, B), BC(B, C), CD(C, D), DA(D, A);
    for(const Segment2D& segment2D : {AB, BC, CD, DA}) {
        for(const Point2D point2D : segment2D.getPointsOnObject(10)) {
            if (!canNodesCoverPoint2D(nodes, point2D)) return false;
        }
    }
    return true;
} 