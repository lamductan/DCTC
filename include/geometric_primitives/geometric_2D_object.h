#ifndef GEOMETRIC_PRIMITIVES_GEOMETRIC_OBJECT_H_
#define GEOMETRIC_PRIMITIVES_GEOMETRIC_OBJECT_H_

#include <iostream>
#include <string>
#include <vector>
#include "geometric_primitives/point2D.h"


class Geometric2DObject {
public:
    virtual bool containsPoint2D(const Point2D& p) const = 0;
    bool containsPoints2D(const std::vector<Point2D>& points) const;
    virtual std::string toString() const = 0;
};

std::vector<Point2D> getPointsInsideGeometric2DObject(
    const Geometric2DObject* geometric_2d_object, const std::vector<Point2D>& points);

#endif //GEOMETRIC_PRIMITIVES_GEOMETRIC_OBJECT_H_