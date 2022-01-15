#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/ray2D.h"


Ray2D::Ray2D() {
    Ray2D ray2D = fromPointAndDirectionVector(Point2D(0, 0), Vector2D(1, 0));
    *this = ray2D;
}

Ray2D::Ray2D(const Line2D& line2D) : Line2D(line2D) {}

Ray2D::Ray2D(const Point2D& p, const Point2D& p1) {
    Ray2D ray2D = fromPointAndDirectionVector(p, Vector2D(p, p1));
    *this = ray2D;
}

Ray2D Ray2D::fromPointAndDirectionVector(const Point2D& p, const Vector2D& direction_vector) {
    Vector2D norm_vector = direction_vector.getNormVector();
    Line2D line2D(p, norm_vector);
    Ray2D ray2D(line2D);
    std::vector<double> v = Vector2DToVector(direction_vector);
    int largest_absolute_value_coef_pos = findLargestAbsoluteValueCoefPos(v);
    ray2D.direction_vector = direction_vector/fabs(v[largest_absolute_value_coef_pos]);
    return ray2D;
}

Ray2D Ray2D::fromTwoPoints(const Point2D& p, const Point2D& p1) {
    return Ray2D(p, p1);
}

Vector2D Ray2D::getDirectionVector() const {return direction_vector;}

Vector2D Ray2D::getUnitVector() const {return direction_vector.getUnitVector();}

bool Ray2D::passesThroughPoint2D(const Point2D& p) const {
    bool do_line_pass_through = Line2D::passesThroughPoint2D(p);
    if (!do_line_pass_through) return false;
    if (p == point0) return true;
    Vector2D v1(p - point0);
    return v1.hasSameDirectionWithVector2D(direction_vector);
}

bool Ray2D::containsPoint2D(const Point2D& p) const {
    return passesThroughPoint2D(p);
}

bool Ray2D::isIdentical(const Ray2D& other) const {
    return (point0 == other.point0 && direction_vector == other.direction_vector);
}

bool Ray2D::intersectsWithLine2D(const Line2D& line2D) const {
    if (!Line2D::intersectsWithLine2D(line2D)) return false;
    Point2D intersection_with_line = Line2D::findIntersectionsWithLine2D(line2D);
    return passesThroughPoint2D(intersection_with_line);
}

Point2D Ray2D::findIntersectionsWithLine2D(const Line2D& line2D) const {
    if (!intersectsWithLine2D(line2D)) return POINT_INFINITY;
    return Line2D::findIntersectionsWithLine2D(line2D); 
}

bool Ray2D::intersectsWithRay2D(const Ray2D& other) const {
    if (direction_vector.hasOppositeDirectionWithVector2D(other.direction_vector)) {
        if (point0 == other.point0) return true;
    }
    if (!Line2D::intersectsWithLine2D(other)) return false;
    Point2D intersection = Line2D::findIntersectionsWithLine2D(other);
    return this->passesThroughPoint2D(intersection) && other.passesThroughPoint2D(intersection);
}

Point2D Ray2D::findIntersectionsWithRay2D(const Ray2D& other) const {
    if (direction_vector.hasOppositeDirectionWithVector2D(other.direction_vector)) {
        if (point0 == other.point0) return point0;
    }
    if (!intersectsWithRay2D(other)) return POINT_INFINITY;
    return Line2D::findIntersectionsWithLine2D(other); 
}

std::vector<Point2D> Ray2D::getPointsOnObject(int n_points) const {
    if (n_points <= 0) return std::vector<Point2D>();
    std::vector<Point2D> points(n_points, Point2D(0, 0));
    double a = coefs[0], b = coefs[1], c = coefs[2];
    if (largest_absolute_value_coef_pos == 1) {
        for(int i = 0; i < n_points; ++i) {
            double new_x = point0.getX() + i;
            points[i].setX(new_x);
            points[i].setY((-c - a*new_x) / b);
        }
    } else {
        for(int i = 0; i < n_points; ++i) {
            double new_y = point0.getY() + i;
            points[i].setY(new_y);
            points[i].setX((-c - b*new_y) / a);
        }
    }
    return points;
}

std::string Ray2D::toString() const {
    std::string s = "Ray2D(";
    s += point0.toString();
    s += ",";
    s += direction_vector.toString();
    s += ")";
    return s;
}