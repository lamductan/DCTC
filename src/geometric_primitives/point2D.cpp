#include <cassert>
#include <cmath>
#include <climits>
#include <string>
#include <sstream>
#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"


Point2D::Point2D() {}

Point2D::Point2D(long double x, long double y) : x(x), y(y) {}

Point2D::Point2D(const Point2D& other) : x(other.x), y(other.y) {}

long double Point2D::getX() const {return x;}
long double Point2D::getY() const {return y;}
int Point2D::getDim() {return dim;}

void Point2D::setX(long double x) {this->x = x;}
void Point2D::setY(long double y) {this->y = y;}

Point2D Point2D::operator+(const Point2D& other) const {
    return Point2D(x + other.x, y + other.y);
}

Point2D Point2D::operator-(const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
}

Point2D Point2D::operator*(long double t) const {
    return Point2D(x*t, y*t);
}

Point2D Point2D::operator/(long double t) const {
    assert(!approx(t, 0.0));
    return Point2D(x/t, y/t);
}

Point2D& Point2D::operator=(const Point2D& other) {
    x = other.x;
    y = other.y;
    return *this;        
}

bool Point2D::operator==(const Point2D& other) const {
    return approx(x, other.x) && approx(y, other.y);
}

bool Point2D::operator<(const Point2D& other) const {
    return (x < other.x) || (x == other.x && y < other.y);
}

bool Point2D::operator>(const Point2D& other) const {
    return other < *this;
}

bool Point2D::operator<=(const Point2D& other) const {
    return (*this < other) || (*this == other);
}

bool Point2D::operator>=(const Point2D& other) const {
    return (*this > other) || (*this == other);
}

long double Point2D::dot(const Point2D& other) const{
    return x*other.x + y*other.y;
}

long double Point2D::cross(const Point2D& other) const{
    return x*other.y - y*other.x; 
}

long double Point2D::computeEuclidDistance(const Point2D& other) const {
    return sqrtl(powl(x - other.x, 2) + powl(y - other.y, 2));
}

bool Point2D::isPointInfinity() const {
    return approx(x, (long double) LONG_LONG_MAX) && approx(y, (long double) LONG_LONG_MAX);
}

std::string Point2D::toString() const{
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(DECIMAL_DIGITS);
    ss << "Point2D(" << x << "," << y << ")";
    return ss.str();
}

Point2D operator*(long double t, const Point2D& p) {
    return p*t;
}

long double computeEuclidDistance(const Point2D& p1, const Point2D& p2) {
    return p1.computeEuclidDistance(p2);
}

std::vector<long double> Point2DToVector(const Point2D& point2D) {
    std::vector<long double> v{point2D.getX(), point2D.getY()};
    return v;
}

Point2D VectorToPoint2D(const std::vector<long double>& v) {
    assert(v.size() >= Point2D::dim);
    return Point2D(v[0], v[1]);
}

std::ostream& operator<<(std::ostream& os, const Point2D& point2D) {
    os << point2D.toString();
    return os;
}

bool almost_eq(const Point2D& p1, const Point2D& p2, long double allowed_diff) {
    return (fabsl(p1.getX() - p2.getX()) <= allowed_diff) && (fabsl(p1.getY() - p2.getY()) <= allowed_diff);
}
