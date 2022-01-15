#include <iostream>
#include <string>
#include <vector>

#include "geometric_primitives/geometric_2D_object.h"
#include "geometric_primitives/point2D.h"


bool Geometric2DObject::containsPoints2D(const std::vector<Point2D>& points) const {
    for(const Point2D& point : points) {
        if (!containsPoint2D(point)) return false;
    }
    return true;
}

std::vector<Point2D> getPointsInsideGeometric2DObject(
    const Geometric2DObject* geometric_2d_object, const std::vector<Point2D>& points
) {
    std::vector<Point2D> points_inside_object;
    for(const Point2D& point : points) {
        if (geometric_2d_object->containsPoint2D(point))
            points_inside_object.push_back(point);
    }
    return points_inside_object;
}