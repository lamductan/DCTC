#ifndef GEOMETRIC_PRIMITIVES_VECTOR2D_H_
#define GEOMETRIC_PRIMITIVES_VECTOR2D_H_

#include <iostream>
#include <string>
#include <vector>
#include "geometric_primitives/point2D.h"


class Vector2D: public Point2D {
public:
    using Point2D::Point2D;
    using Point2D::operator=;
    using Point2D::operator==;
    Vector2D(const Point2D& p);
    Vector2D(const Point2D& a, const Point2D& b);
    double computeNorm() const;
    double length() const;
    Vector2D getUnitVector() const;
    Vector2D getNormVector() const;
    virtual std::string toString() const;
    bool isZeroVector() const;
    bool hasSameDirectionWithVector2D(const Vector2D& other) const;
    bool hasOppositeDirectionWithVector2D(const Vector2D& other) const;
    bool isParallelWithVector2D(const Vector2D& other) const;
};

std::vector<double> Vector2DToVector(const Vector2D& vector2D);
Vector2D VectorToVector2D(const std::vector<double>& v);

#endif //GEOMETRIC_PRIMITIVES_VECTOR2D_H_