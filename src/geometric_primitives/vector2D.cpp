#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/vector2D.h"

Vector2D::Vector2D(const Point2D& p) {
    x = p.getX();
    y = p.getY();
}

Vector2D::Vector2D(const Point2D& a, const Point2D& b) {
    x = b.getX() - a.getX();
    y = b.getY() - a.getY();
}

long double Vector2D::computeNorm() const {
    return sqrtl(powl(x, 2) + powl(y, 2));
}

long double Vector2D::length() const {
    return sqrtl(powl(x, 2) + powl(y, 2));
}

Vector2D Vector2D::getNormVector() const {
    return Vector2D(y, -x);
}

Vector2D Vector2D::getUnitVector() const {
    long double norm_value = computeNorm();
    assert(norm_value > EPSILON);
    return Vector2D(x/norm_value, y/norm_value);
}

std::string Vector2D::toString() const {
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(DECIMAL_DIGITS);
    ss << "Vector2D(" << x << "," << y << ")";
    return ss.str();
}

bool Vector2D::isZeroVector() const {
    return fabsl(x) <= EPSILON && fabsl(y) <= EPSILON;
}
bool Vector2D::hasSameDirectionWithVector2D(const Vector2D& other) const {
    if (isZeroVector() || other.isZeroVector()) return true;
    std::vector<long double> v1 = Vector2DToVector(*this);
    std::vector<long double> v2 = Vector2DToVector(other);
    int largest_absolute_value_coef_pos = findLargestAbsoluteValueCoefPos(v1);
    long double k = v2[largest_absolute_value_coef_pos]/v1[largest_absolute_value_coef_pos];
    if (k < -EPSILON) return false;
    int other_pos = 1 - largest_absolute_value_coef_pos;
    return approx(v2[other_pos], v1[other_pos]*k);
}

bool Vector2D::hasOppositeDirectionWithVector2D(const Vector2D& other) const {
    if (isZeroVector() || other.isZeroVector()) return true;
    std::vector<long double> v1 = Vector2DToVector(*this);
    std::vector<long double> v2 = Vector2DToVector(other);
    int largest_absolute_value_coef_pos = findLargestAbsoluteValueCoefPos(v1);
    long double k = v2[largest_absolute_value_coef_pos]/v1[largest_absolute_value_coef_pos];
    if (k > EPSILON) return false;
    int other_pos = 1 - largest_absolute_value_coef_pos;
    return approx(v2[other_pos], v1[other_pos]*k);
}

bool Vector2D::isParallelWithVector2D(const Vector2D& other) const {
    return hasSameDirectionWithVector2D(other) || hasOppositeDirectionWithVector2D(other);
}

std::vector<long double> Vector2DToVector(const Vector2D& vector2D) {
    std::vector<long double> v{vector2D.getX(), vector2D.getY()};
    return v;
}

Vector2D VectorToVector2D(const std::vector<long double>& v) {
    assert(v.size() >= Vector2D::dim);
    return Vector2D(v[0], v[1]);
}