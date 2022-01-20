#include <cmath>

#include "equations/quadratic_equation.h"
#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/geometric_2D_object.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/circle.h"


Circle::Circle() {
    init(Point2D(0, 0), 1);
}

Circle::Circle(const Point2D& center, double radius) {
    init(center, radius);
}

void Circle::init(const Point2D& center, double radius) {
    this->center = center;
    this->radius = radius;
    radius_squared = pow(radius, 2);
}

Point2D Circle::getCenter() const {return center;}

double Circle::getRadius() const {return radius;}

void Circle::setRadius(double radius) {this->radius = radius;}

double Circle::operator()(const Point2D& p) const {
    double dx = p.getX() - center.getX();
    double dy = p.getY() - center.getY();
    return pow(dx, 2) + pow(dy, 2) - pow(radius, 2);
}

bool Circle::passesThroughPoint2D(const Point2D& p) const {
    return approx(operator()(p), 0);
}

bool Circle::containsPoint2D(const Point2D& p) const {
    return operator()(p) <= EPSILON;
}

bool Circle::containsPoint2DWithInfRange(const Point2D& p) const {
    return true;
}

bool Circle::intersectsWithLine2D(const Line2D& line2D) const {
    int line_largest_absolute_value_coef_pos = line2D.getLargestAbsoluteValueCoefPos();
    std::vector<double> line_coefs = line2D.getCoefs();
    double a = line_coefs[0], b = line_coefs[1], c = line_coefs[2];
    double ox = center.getX();
    double oy = center.getY();
    if (line_largest_absolute_value_coef_pos == 0) {
        std::vector<double> line_1_coefs{b, a, c};
        Line2D line2D_1 = line2D.fromCoefficients(line_1_coefs);
        Point2D center_1(oy, ox);
        Circle circle_1(center_1, radius);
        return circle_1.intersectsWithLine2D(line2D_1);
    }
    double eq_a = pow(a/b, 2) + 1;
    double eq_b = 2.0*(a/b*(c/b + oy) - ox);
    double eq_c = pow(ox, 2) + pow(c/b + oy, 2) - pow(radius, 2);
    QuadraticEquation quadratic_equation(eq_a, eq_b, eq_c);
    return quadratic_equation.getNumberOfSolutions() > 0;    
}

std::vector<Point2D> Circle::findIntersectionsWithLine2D(const Line2D& line2D) const {
    int line_largest_absolute_value_coef_pos = line2D.getLargestAbsoluteValueCoefPos();
    std::vector<double> line_coefs = line2D.getCoefs();
    double a = line_coefs[0], b = line_coefs[1], c = line_coefs[2];
    double ox = center.getX();
    double oy = center.getY();
    if (line_largest_absolute_value_coef_pos == 0 && (fabs(a) - fabs(b) > EPSILON)) {
        std::vector<double> line_1_coefs{b, a, c};
        Line2D line2D_1 = line2D.fromCoefficients(line_1_coefs);
        Point2D center_1(oy, ox);
        Circle circle_1(center_1, radius);
        std::vector<Point2D> intersections_1 = circle_1.findIntersectionsWithLine2D(line2D_1);
        int n_intersections = intersections_1.size();
        if (n_intersections == 0) return intersections_1;
        if (n_intersections == 1) {
            Point2D intersection(intersections_1[0].getY(), intersections_1[0].getX());
            return {intersection};
        }
        Point2D intersection1(intersections_1[0].getY(), intersections_1[0].getX());
        Point2D intersection2(intersections_1[1].getY(), intersections_1[1].getX());
        if (intersection1.getX() > intersection2.getX()) std::swap(intersection1, intersection2);
        return {intersection1, intersection2};
    }
    double eq_a = pow(a/b, 2) + 1;
    double eq_b = 2.0*(a/b*(c/b + oy) - ox);
    double eq_c = pow(ox, 2) + pow(c/b + oy, 2) - pow(radius, 2);
    QuadraticEquation quadratic_equation(eq_a, eq_b, eq_c);

    int n_intersections = quadratic_equation.getNumberOfSolutions();
    if (n_intersections == 0)
        return {};

    std::pair<double, double> xs = quadratic_equation.solve();
    Point2D intersection1(xs.first, (-c - a*xs.first)/b);
    if (n_intersections == 1) {
        return {intersection1};
    }
    Point2D intersection2(xs.second, (-c - a*xs.second)/b);
    if (intersection1.getX() > intersection2.getX()) std::swap(intersection1, intersection2);
    if (intersection1 == intersection2) intersection2 = POINT_INFINITY;
    std::vector<Point2D> ans = std::vector<Point2D>{intersection1, intersection2};
    return ans;
}

bool Circle::intersectsWithSegment2D(const Segment2D& segment2D) const {
    std::vector<Point2D> intersections = findIntersectionsWithSegment2D(segment2D);
    return !intersections.empty();
}

std::vector<Point2D> Circle::findIntersectionsWithSegment2D(const Segment2D& segment2D) const {
    std::vector<Point2D> intersections = findIntersectionsWithLine2D(segment2D);
    return getPointsInsideGeometric2DObject(&segment2D, intersections);
}

bool Circle::intersectsWithRay2D(const Ray2D& ray2D) const {
    std::vector<Point2D> intersections = findIntersectionsWithRay2D(ray2D);
    return !intersections.empty();
}

std::vector<Point2D> Circle::findIntersectionsWithRay2D(const Ray2D& ray2D) const {
    std::vector<Point2D> intersections = findIntersectionsWithLine2D(ray2D);
    return getPointsInsideGeometric2DObject(&ray2D, intersections);
}

bool Circle::intersectsWithCircle(const Circle& other) const {
    std::vector<Point2D> intersections = findIntersectionsWithCircle(other);
    return !intersections.empty();
}

std::vector<Point2D> Circle::findIntersectionsWithCircle(const Circle& other) const {
    double a1 = center.getX(), b1 = center.getY(), c1 = radius;
    double a2 = other.center.getX(), b2 = other.center.getY(), c2 = other.radius;
    Line2D radical_line = Line2D::fromCoefficients(std::vector<double>{
        2.0*(a1 - a2), 2.0*(b1 - b2), (a2*a2 - a1*a1 + b2*b2 - b1*b1 - c2*c2 + c1*c1)});
    return findIntersectionsWithLine2D(radical_line);
}

std::string Circle::toString() const {
    char s[100];
    snprintf(s, 100, "Circle(%s,%.2f)", center.toString().c_str(), radius);
    std::string str = s;
    return str;
}

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
    os << circle.toString();
    return os;
}