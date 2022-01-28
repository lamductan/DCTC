#include <cassert>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/line2D.h"

int Line2D::getDim() {return dim;}

void Line2D::init(const Point2D& point0, const Vector2D& norm_vector) {
    this->point0 = point0;
    std::vector<long double> norm_v_original = Vector2DToVector(norm_vector);
    largest_absolute_value_coef_pos = findLargestAbsoluteValueCoefPos(norm_v_original);
    long double k = fabsl(norm_v_original[largest_absolute_value_coef_pos]);
    if (k <= EPSILON) std::cout << "k = " << k << '\n';
    assert (k > EPSILON);
    this->norm_vector = norm_vector / k;
    long double c = -this->norm_vector.dot(point0);
    std::vector<long double> coefs = Vector2DToVector(this->norm_vector);
    coefs.push_back(c);
    this->coefs = coefs;
}

Line2D::Line2D(const Point2D& point0, const Vector2D& norm_vector) {
    init(point0, norm_vector);
}

Line2D::Line2D(const Line2D& other) {
    *this = other;
}

Line2D Line2D::fromCoefficients(const std::vector<long double>& coefs) {
    assert(coefs.size() >= Line2D::dim + 1);
    std::vector<long double> norm_v_original(coefs.begin(), coefs.begin() + Vector2D::dim);
    int largest_absolute_value_coef_pos = findLargestAbsoluteValueCoefPos(norm_v_original);
    long double c = coefs[Line2D::dim] / norm_v_original[largest_absolute_value_coef_pos]; 
    std::vector<long double> point0_v(Point2D::dim, 0);
    point0_v[largest_absolute_value_coef_pos] = -c;
    Point2D point0 = VectorToPoint2D(point0_v);
    Vector2D norm_vector = VectorToVector2D(norm_v_original);
    return Line2D(point0, norm_vector);
}

Line2D Line2D::fromTwoPoints(const Point2D& p1, const Point2D& p2) {
    Vector2D direction_vector = Vector2D(p2 - p1);
    Vector2D norm_vector = direction_vector.getNormVector();
    return Line2D(p1, norm_vector);
}

std::vector<long double> Line2D::getCoefs() const {return coefs;}

Point2D Line2D::getPoint0() const {return point0;}

Vector2D Line2D::getNormVector() const {return norm_vector;}

int Line2D::getLargestAbsoluteValueCoefPos() const {return largest_absolute_value_coef_pos;}

long double Line2D::operator()(const Point2D& p) const {
    return norm_vector.dot(p) + coefs.back();
}

bool Line2D::passesThroughPoint2D(const Point2D& p) const {
    return approx(operator()(p), 0.0);
}

bool Line2D::containsPoint2D(const Point2D& p) const {
    return passesThroughPoint2D(p);
}

bool Line2D::hasParallelNormVectorWithLine2D(const Line2D& other) const {
    return norm_vector.isParallelWithVector2D(other.norm_vector);
}

bool Line2D::isParallelWithLine2D(const Line2D& other) const {
    return hasParallelNormVectorWithLine2D(other) && !other.passesThroughPoint2D(point0);
}

bool Line2D::isIdentical(const Line2D& other) const {
    return hasParallelNormVectorWithLine2D(other) && other.passesThroughPoint2D(point0);
}

bool Line2D::intersectsWithLine2D(const Line2D& other) const {
    return !isParallelWithLine2D(other);
}

Point2D Line2D::findIntersectionsWithLine2D(const Line2D& other) const {
    if (!Line2D::intersectsWithLine2D(other)) return POINT_INFINITY;
    long double a1 = coefs[0], b1 = coefs[1], c1 = coefs[2];
    long double a2 = other.coefs[0], b2 = other.coefs[1], c2 = other.coefs[2];
    long double D = (a1*b2 - a2*b1);
    return Point2D(-(c1*b2 - c2*b1)/D, (c1*a2 - c2*a1)/D);
}

long double Line2D::distanceToPoint2D(const Point2D& p) const {
    return fabsl(operator()(p))/norm_vector.computeNorm();
}

long double Line2D::distanceToLine2D(const Line2D& other) const {
    if (!isParallelWithLine2D(other)) return 0;
    return other.distanceToPoint2D(point0);
}

std::vector<Point2D> Line2D::getPointsOnObject(int n_points) const {
    if (n_points <= 0) return std::vector<Point2D>();
    std::vector<Point2D> points(n_points, Point2D(0, 0));
    long double a = coefs[0], b = coefs[1], c = coefs[2];
    for(int i = 0; i < n_points; ++i) {
        if (largest_absolute_value_coef_pos == 1) {
            points[i].setX(i);
            points[i].setY((-c - a*i) / b);
        } else {
            points[i].setY(i);
            points[i].setX((-c - b*i) / a);
        } 
    }
    return points;
}

std::string Line2D::toString() const {
    char s[100];
    snprintf(s, 100, "Line2D(%.2f*x + %.2f*y + %.2f = 0)", (double) coefs[0], (double) coefs[1], (double) coefs[2]);
    std::string str = s;
    return str;
}

std::ostream& operator<<(std::ostream& os, const Line2D& line2D) {
    os << line2D.toString();
    return os;
}