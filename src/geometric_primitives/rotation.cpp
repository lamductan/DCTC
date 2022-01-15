#include <cmath>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/rotation.h"


Point2D rotate(const Point2D& A, double angle) {
    angle = standardize_angle(angle);
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);
    double x = A.getX();
    double y = A.getY();
    return Point2D(x*cos_angle - y*sin_angle, x*sin_angle + y*cos_angle);
}

Point2D rotate(const Point2D& A, const Point2D& O, double angle) {
    Point2D A1 = A - O;
    Point2D R1 = rotate(A1, angle);
    return R1 + O;
}