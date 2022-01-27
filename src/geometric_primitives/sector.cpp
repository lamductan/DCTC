#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/circle.h"
#include "geometric_primitives/rotation.h"
#include "geometric_primitives/geom_utils_1.h"
#include "geometric_primitives/sector.h"


Sector::Sector() {
    init(Point2D(0, 0), 1, PI_2, 0);
}

Sector::Sector(const Point2D& center, long double radius, long double angle, long double orientation_angle) {
    init(center, radius, angle, orientation_angle);
}

void Sector::init(const Point2D& center, long double radius, long double angle, long double orientation_angle) {
    Circle::init(center, radius);
    original_bisector_unit_point = center + Point2D(1, 0);
    this->angle = standardize_angle(angle);
    this->half_angle = this->angle/2;
    orientWithAngle(orientation_angle);
}

void Sector::computeMidpointAndEndpoints() {
    midpoint = getPointOnRayAtDistance(bisector_ray, radius);
    endpoint1 = rotate(midpoint, center, -half_angle);
    endpoint2 = rotate(midpoint, center, half_angle);
    boundary1 = Segment2D(center, endpoint1);
    boundary2 = Segment2D(center, endpoint2);
}

void Sector::orientWithAngle(long double orientation_angle) {
    orientation_angle = standardize_angle(orientation_angle);
    this->orientation_angle = orientation_angle;
    bisector_unit_point = rotate(original_bisector_unit_point, center, orientation_angle);
    bisector_unit_vector = Vector2D(center, bisector_unit_point);
    bisector_ray = Ray2D::fromPointAndDirectionVector(center, bisector_unit_vector);
    computeMidpointAndEndpoints();
}

void Sector::setRadius(long double radius) {
    this->radius = radius;
    computeMidpointAndEndpoints();
}

Point2D Sector::getOriginalBisectorUnitPoint() const {return original_bisector_unit_point;}

Point2D Sector::getBisectorUnitPoint() const {return bisector_unit_point;}

long double Sector::getAngle() const {return angle;}

long double Sector::getOrientationAngle() const {return orientation_angle;}

Point2D Sector::getEndpoint1() const {return endpoint1;}

Point2D Sector::getEndpoint2() const {return endpoint2;}

Point2D Sector::getMidPoint() const {return midpoint;}

Vector2D Sector::getBisectorUnitVector() const {return bisector_unit_vector;}

Ray2D Sector::getBisectorRay() const {return bisector_ray;}

Segment2D Sector::getBoundarySegment1() const {return boundary1;}

Segment2D Sector::getBoundarySegment2() const {return boundary2;}

bool Sector::passesThroughPoint2D(const Point2D& p) const {
    return Circle::passesThroughPoint2D(p) && containsPoint2D(p);
}

bool Sector::containsPoint2D(const Point2D& p) const {
    return Circle::containsPoint2D(p) && containsPoint2DWithInfRange(p);
}

bool Sector::containsPoint2DWithInfRange(const Point2D& p) const {
    Vector2D v(center, p);
    long double criterion = v.dot(bisector_unit_vector) - v.length()*cosl(half_angle);
    //std::cout << "Check inside sector: " << *this << ", " << p << ' ' << criterion << '\n';
    return criterion >= -EPSILON;
}

std::vector<Point2D> Sector::findIntersectionsSectorWithLine2D(const Line2D& line2D) const {
    std::vector<Point2D> intersections_with_circle = Circle::findIntersectionsWithLine2D(line2D);
    print_vector<Point2D>(intersections_with_circle, '\n');
    return intersections_with_circle;
    Point2D intersection_with_boundary_1 = boundary1.findIntersectionsWithLine2D(line2D);
    Point2D intersection_with_boundary_2 = boundary2.findIntersectionsWithLine2D(line2D);
    std::vector<Point2D> intersections = intersections_with_circle 
        + std::vector<Point2D>{intersection_with_boundary_1, intersection_with_boundary_2};
    print_vector<Point2D>(intersections, '\n');
    return getPointsInsideGeometric2DObject(this, intersections);
}

std::vector<Point2D> Sector::findIntersectionsSectorWithRay2D(const Ray2D& ray2D) const {
    std::vector<Point2D> intersections_with_circle = Circle::findIntersectionsWithLine2D(ray2D);
    std::vector<Point2D> intersection_with_boundary_1 = findIntersectionsBetweenGeom2DObjects(boundary1, ray2D);
    std::vector<Point2D> intersection_with_boundary_2 = findIntersectionsBetweenGeom2DObjects(boundary2, ray2D);
    std::vector<Point2D> intersections = intersections_with_circle 
        + intersection_with_boundary_1 + intersection_with_boundary_2;
    return getPointsInsideGeometric2DObject(this, intersections);
}

std::vector<Point2D> Sector::findIntersectionsSectorWithSegment2D(const Segment2D& segment2D) const {
    std::vector<Point2D> intersections_with_circle = Circle::findIntersectionsWithLine2D(segment2D);
    std::vector<Point2D> intersection_with_boundary_1 = findIntersectionsBetweenGeom2DObjects(boundary1, segment2D);
    std::vector<Point2D> intersection_with_boundary_2 = findIntersectionsBetweenGeom2DObjects(boundary2, segment2D);
    std::vector<Point2D> intersections = intersections_with_circle 
        + intersection_with_boundary_1 + intersection_with_boundary_2;
    return getPointsInsideGeometric2DObject(this, intersections);
}

std::vector<Point2D> Sector::findIntersectionsSectorWithCircle(const Circle& circle) const {
    std::vector<Point2D> intersections_with_circle = Circle::findIntersectionsWithCircle(circle);
    std::vector<Point2D> intersections_with_boundary_1 = circle.findIntersectionsWithSegment2D(boundary1);
    std::vector<Point2D> intersections_with_boundary_2 = circle.findIntersectionsWithSegment2D(boundary2);
    std::vector<Point2D> intersections = intersections_with_circle 
        + intersections_with_boundary_1 + intersections_with_boundary_2;
    return getPointsInsideGeometric2DObject(this, intersections);
}

bool Sector::arcIntersectsWithLine2D(const Line2D& line2D) const {
    return !findIntersectionsArcWithLine2D(line2D).empty();
}

std::vector<Point2D> Sector::findIntersectionsArcWithLine2D(const Line2D& line2D) const {
    std::vector<Point2D> intersections = Circle::findIntersectionsWithLine2D(line2D);
    return getPointsInsideGeometric2DObject(this, intersections);
}

bool Sector::arcIntersectsWithRay2D(const Ray2D& ray2D) const {
    return !findIntersectionsArcWithRay2D(ray2D).empty();
}

std::vector<Point2D> Sector::findIntersectionsArcWithRay2D(const Ray2D& ray2D) const {
    std::vector<Point2D> intersections = Circle::findIntersectionsWithRay2D(ray2D);
    return getPointsInsideGeometric2DObject(this, intersections);
}

bool Sector::arcIntersectsWithSegment2D(const Segment2D& segment2D) const {
    return !findIntersectionsArcWithSegment2D(segment2D).empty();
}

std::vector<Point2D> Sector::findIntersectionsArcWithSegment2D(const Segment2D& segment2D) const {
    std::vector<Point2D> intersections = Circle::findIntersectionsWithSegment2D(segment2D);
    return getPointsInsideGeometric2DObject(this, intersections);
}

bool Sector::arcIntersectsWithCircle(const Circle& circle) const {
    return !findIntersectionsArcWithCircle(circle).empty();
}

std::vector<Point2D> Sector::findIntersectionsArcWithCircle(const Circle& circle) const {
    std::vector<Point2D> intersections = Circle::findIntersectionsWithCircle(circle);
    return getPointsInsideGeometric2DObject(this, intersections);
}

bool Sector::arcIntersectsWithArc(const Sector& other) const {
    return !findIntersectionsArcWithArc(other).empty();
}

std::vector<Point2D> Sector::findIntersectionsArcWithArc(const Sector& other) const {
    std::vector<Point2D> intersections = Circle::findIntersectionsWithCircle(other);
    return getPointsInsideGeometric2DObject(this, getPointsInsideGeometric2DObject(&other, intersections));
}

bool Sector::sectorIntersectsWithArc(const Sector& other) const {
    return !findIntersectionsSectorWithArc(other).empty();
}

std::vector<Point2D> Sector::findIntersectionsSectorWithArc(const Sector& other) const {
    std::vector<Point2D> intersections_with_circle = findIntersectionsSectorWithCircle(other);
    std::vector<Point2D> intersections_with_boundary_1 = other.findIntersectionsArcWithSegment2D(boundary1);
    std::vector<Point2D> intersections_with_boundary_2 = other.findIntersectionsArcWithSegment2D(boundary2);
    std::vector<Point2D> intersections = intersections_with_circle 
        + intersections_with_boundary_1 + intersections_with_boundary_2;
    return getPointsInsideGeometric2DObject(this, getPointsInsideGeometric2DObject(&other, intersections));
}

bool Sector::arcIntersectsWithSector(const Sector& other) const {
    return other.sectorIntersectsWithArc(*this);
}

std::vector<Point2D> Sector::findIntersectionsArcWithSector(const Sector& other) const {
    return other.findIntersectionsSectorWithArc(*this);
}

bool Sector::sectorIntersectsWithSector(const Sector& other) const {
    return !findIntersectionsSectorWithSector(other).empty();
}

std::vector<Point2D> Sector::findIntersectionsSectorWithSector(const Sector& other) const {
    std::vector<Point2D> intersections_arc_other = findIntersectionsArcWithSector(other);
    std::vector<Point2D> intersections_boundary1_other = other.findIntersectionsSectorWithSegment2D(boundary1);
    std::vector<Point2D> intersections_boundary2_other = other.findIntersectionsSectorWithSegment2D(boundary2);
    return intersections_arc_other + intersections_boundary1_other + intersections_boundary2_other;
}

bool Sector::canOrientToCoverPoint2D(const Point2D& p, bool consider_distance) const {
    if (consider_distance && computeEuclidDistance(center, p) > radius + EPSILON) return false;
    return true;
}

long double Sector::orientToCoverPoint2D(const Point2D& p, bool consider_distance) {
    if (!canOrientToCoverPoint2D(p, consider_distance)) return -1;
    long double orient_angle = computeAngle(original_bisector_unit_point, center, p);
    orientWithAngle(orient_angle);
    return orient_angle;
}

long double Sector::findPossibleOrientAngleToCoverPoints2D(const std::vector<Point2D>& points, bool consider_distance) const {
    std::vector<long double> angles;
    int n = points.size();
    for(const Point2D& point : points) {
        if (consider_distance && computeEuclidDistance(center, point) > radius) return false;
        long double angle_with_original_unit_vector = computeAngle(original_bisector_unit_point, center, point);
        angles.push_back(angle_with_original_unit_vector);
    }
    sort(angles.begin(), angles.end());
    for(int i = 0; i < n; ++i) {
        angles.push_back(angles[i] + TWO_PI);
    }
    int n_points_can_cover = 0;
    for(int i = 0; i < n; ++i) {
        if (angles[i + n - 1] - angles[i] <= this->angle + EPSILON) 
            return standardize_angle(angles[i] + half_angle);
    }
    return -1;
}

bool Sector::canOrientToCoverPoints2D(const std::vector<Point2D>& points, bool consider_distance) const {
    return findPossibleOrientAngleToCoverPoints2D(points, consider_distance) != -1;
}

long double Sector::orientToCoverPoints2D(const std::vector<Point2D>& points, bool consider_distance) {
    long double orient_angle = findPossibleOrientAngleToCoverPoints2D(points, consider_distance);
    if (orient_angle == -1) return -1;
    orientWithAngle(orient_angle);
    return orient_angle;
}

long double Sector::orientBoundaryPassingThroughPointAndCoverAnotherPoint(
    const Point2D& a, const Point2D& b, bool consider_distance
) {
    long double angle_AOB = computeGeometricAngle(a, center, b);
    if (angle_AOB > angle + EPSILON) return -1;
    long double required_radius = std::max(computeEuclidDistance(center, a), computeEuclidDistance(center, b));
    if (consider_distance && radius < required_radius) return -1;
    long double old_radius = (!consider_distance && radius < required_radius) ? radius : -1;
    if (old_radius != -1) setRadius(required_radius);
    long double angle_xOA = computeAngle(original_bisector_unit_point, center, a);
    long double candidate_orientation_angle_1 = standardize_angle(angle_xOA - half_angle);
    long double candidate_orientation_angle_2 = standardize_angle(angle_xOA + half_angle);
    for(long double candidate_orientation_angle : {candidate_orientation_angle_1, candidate_orientation_angle_2}) {
        orientWithAngle(candidate_orientation_angle);
        if (containsPoint2D(b)) break;
    }
    if (old_radius != -1) setRadius(old_radius);
    return orientation_angle;
}

std::string Sector::toString() const {
    char s[100];
    snprintf(s, 100, "Sector(%s,%.2f,%.2f,%.2f)",
        center.toString().c_str(), (double) radius, (double) rad2deg(angle), (double) orientation_angle);
    std::string str = s;
    return str;
}