#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/circle.h"
#include "geometric_primitives/sector.h"
#include "geometric_primitives/geom2D.h"
#include "geometric_primitives/rotation.h"


double computeCCW(const Point2D& a, const Point2D& b, const Point2D& c) {
    //return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX())*(b.getY() - a.getY());
    return Vector2D(a, b).cross(Vector2D(a, c));
}

double computeCCW(const Vector2D& a, const Vector2D& b) {
    //return a.getX()*b.getY() - b.getX()*a.getY();
    return a.cross(b);
}

double computeAngle(const Vector2D& a, const Vector2D& b) {
    double ccw = computeCCW(a, b);
    double norm_a = a.computeNorm();
    if (approx(norm_a, 0)) return 0;
    double norm_b = b.computeNorm();
    if (approx(norm_b, 0)) return 0;
    double dot = a.dot(b);
    double cos_alpha = dot/(norm_a * norm_b);
    if (cos_alpha > 1) cos_alpha = 1;
    else if (cos_alpha < -1) cos_alpha = -1;
    double angle = acos(cos_alpha);
    if (fabs(angle) <= EPSILON) return 0;
    if (ccw < 0) angle = TWO_PI - angle;
    return angle;
}

double computeAngle(const Point2D& b, const Point2D& a, const Point2D& c) {
    Vector2D vector_ab = Vector2D(a, b);
    Vector2D vector_ac = Vector2D(a, c);
    return computeAngle(vector_ab, vector_ac);
}

Ray2D getBisectorOfAngle(const Point2D& b, const Point2D& a, const Point2D& c) {
    Vector2D vector_ab = Vector2D(a, b);
    Vector2D vector_ac = Vector2D(a, c);
    double angle_bac = computeAngle(vector_ab, vector_ac);
    double half_bac = angle_bac/2;
    Ray2D ray_ab = Ray2D::fromTwoPoints(a, b);
    Point2D P_ab = getPointOnRayAtDistance(ray_ab, 1);
    Point2D P_bisector = rotate(P_ab, a, half_bac);
    return Ray2D::fromTwoPoints(a, P_bisector);
}

double computeGeometricAngle(const Vector2D& a, const Vector2D& b) {
    double angle = computeAngle(a, b);
    return computeGeometricAngle(angle);
}

double computeGeometricAngle(const Point2D& b, const Point2D& a, const Point2D& c) {
    double angle = computeAngle(b, a, c);
    return computeGeometricAngle(angle);
}

double computeAngle(const Ray2D& r1, const Ray2D& r2) {
    return computeAngle(r1.getDirectionVector(), r2.getDirectionVector());
}

double computeAngle(const Ray2D& r, const Vector2D& v) {
    return computeAngle(r.getDirectionVector(), v);
}

double computeAngle(const Vector2D& v, const Ray2D& r) {
    return computeAngle(r.getDirectionVector(), v);
}

double computeAngle(const Line2D& l1, const Line2D& l2) {
    return computeGeometricAngle(computeAngle(l1.getNormVector(), l2.getNormVector()));
}

double computeOppositeAngle(double angle) {return standardize_angle(angle + PI);}

Point2D getPointOnRayAtDistance(const Ray2D& ray2D, double distance) {
    Circle circle(ray2D.getPoint0(), distance);
    return circle.findIntersectionsWithRay2D(ray2D)[0];
}

Point2D getPointOnSegmentAtDistanceFromEndpoint1(const Segment2D& segment2D, double distance) {
    if (distance > segment2D.length()) return POINT_INFINITY;
    Circle circle(segment2D.getEndpoint1(), distance);
    return circle.findIntersectionsWithSegment2D(segment2D)[0];
}

Point2D getPointOnSegmentAtDistanceFromEndpoint2(const Segment2D& segment2D, double distance) {
    if (distance > segment2D.length()) return POINT_INFINITY;
    Circle circle(segment2D.getEndpoint2(), distance);
    return circle.findIntersectionsWithSegment2D(segment2D)[0];
}

Line2D getLine2DParallelLineAndPassingThroughPoint(const Line2D& line2D, const Point2D& p) {
    return Line2D(p, line2D.getNormVector());
}

Line2D getLine2DPerpendicularLineAndPassingThroughPoint(const Line2D& line2D, const Point2D& p) {
    return Line2D(p, line2D.getNormVector().getNormVector());
}

bool isLineParallelLine(const Line2D& l1, const Line2D& l2) {
    return approx(computeAngle(l1, l2), 0);
}

bool isLinePerpendicularLine(const Line2D& l1, const Line2D& l2) {
    return approx(computeAngle(l1, l2), PI_2);
}

std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Ray2D& ray2D, const Segment2D& segment2D) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
    Point2D intersection = ray2D.findIntersectionsWithLine2D(segment2D);
    std::vector<Point2D> intersections;
    if (segment2D.containsPoint2D(intersection)) intersections.push_back(intersection);
    return intersections;
}

std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Segment2D& segment2D, const Ray2D& ray2D) {
    return findIntersectionsBetweenGeom2DObjects(ray2D, segment2D);
}

std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Segment2D& segment2D_1, const Segment2D& segment2D_2){
    Point2D intersection = segment2D_1.findIntersectionsWithSegment2D(segment2D_2);
    std::vector<Point2D> intersections;
    if (!intersection.isPointInfinity()) intersections.push_back(intersection);
    return intersections;
}