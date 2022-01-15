#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/geom_utils_1.h"


Segment2D::Segment2D() {
    Segment2D segment2D = fromTwoPoints(Point2D(0, 0), Point2D(1, 0));
    *this = segment2D;
}

Segment2D::Segment2D(const Point2D& p1, const Point2D& p2) {
    Segment2D segment2D = fromTwoPoints(p1, p2);
    *this = segment2D;
}

Segment2D::Segment2D(const Line2D& line2D) : Line2D(line2D) {}

Segment2D Segment2D::fromTwoPoints(const Point2D& p1, const Point2D& p2) {
    Line2D line2D = Line2D::fromTwoPoints(p1, p2);
    Segment2D segment2D(line2D);
    segment2D.endpoint1 = p1;
    segment2D.endpoint2 = p2;
    return segment2D;
}

double Segment2D::length() const {
    return endpoint1.computeEuclidDistance(endpoint2);
}

Point2D Segment2D::getEndpoint1() const {return endpoint1;}

Point2D Segment2D::getEndpoint2() const {return endpoint2;}

Point2D Segment2D::getMidPoint() const {return (endpoint1 + endpoint2) / 2;}

bool Segment2D::passesThroughPoint2D(const Point2D& p) const {
    bool do_line_pass_through = Line2D::passesThroughPoint2D(p);
    if (!do_line_pass_through) return false;
    Vector2D v1 = Vector2D(p - endpoint1);
    Vector2D v2 = Vector2D(p - endpoint2);
    return v2.hasOppositeDirectionWithVector2D(v1);
}

bool Segment2D::containsPoint2D(const Point2D& p) const {
    return passesThroughPoint2D(p);
}

bool Segment2D::isIdentical(const Segment2D& other) const {
    return (endpoint1 == other.endpoint1 && endpoint2 == other.endpoint2) 
        || (endpoint1 == other.endpoint2 && endpoint2 == other.endpoint1);
}

bool Segment2D::intersectsWithLine2D(const Line2D& line2D) const {
    if (!Line2D::intersectsWithLine2D(line2D)) return false;
    Point2D intersection_with_line = Line2D::findIntersectionsWithLine2D(line2D);
    return passesThroughPoint2D(intersection_with_line);
}

Point2D Segment2D::findIntersectionsWithLine2D(const Line2D& line2D) const {
    if (!intersectsWithLine2D(line2D)) return POINT_INFINITY;
    return Line2D::findIntersectionsWithLine2D(line2D); 
}

bool Segment2D::intersectsWithSegment2D(const Segment2D& other) const {
    if (!Line2D::intersectsWithLine2D(other)) return false;
    Point2D intersection = Line2D::findIntersectionsWithLine2D(other);
    return this->passesThroughPoint2D(intersection) && other.passesThroughPoint2D(intersection);
}

Point2D Segment2D::findIntersectionsWithSegment2D(const Segment2D& other) const {
    if (!intersectsWithSegment2D(other)) return POINT_INFINITY;
    return Line2D::findIntersectionsWithLine2D(other); 
}

Line2D Segment2D::getPerpendicularBisector() const {
    return getLine2DPerpendicularLineAndPassingThroughPoint(*this, getMidPoint());
}

std::vector<Point2D> Segment2D::getPointsOnObject(int n_points) const {
    if (n_points <= 0) return std::vector<Point2D>();
    std::vector<Point2D> points(n_points, Point2D(0, 0));
    double a = coefs[0], b = coefs[1], c = coefs[2];
    if (largest_absolute_value_coef_pos == 1) {
        double delta_x = (endpoint2.getX() - endpoint1.getX())/n_points;
        for(int i = 0; i < n_points; ++i) {
            double new_x = endpoint1.getX() + delta_x*i;
            points[i].setX(new_x);
            points[i].setY((-c - a*new_x) / b);
        }
    } else {
        double delta_y = (endpoint2.getY() - endpoint1.getY())/n_points;
        for(int i = 0; i < n_points; ++i) {
            double new_y = endpoint1.getY() + delta_y*i;
            points[i].setY(new_y);
            points[i].setX((-c - b*new_y) / a);
        }
    }
    return points;
}

std::string Segment2D::toString() const {
    std::string s = "Segment2D(";
    s += endpoint1.toString();
    s += ",";
    s += endpoint2.toString();
    s += ")";
    return s;
}
