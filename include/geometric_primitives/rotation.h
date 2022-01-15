#ifndef GEOMETRIC_PRIMITIVES_ROTATION_H_
#define GEOMETRIC_PRIMITIVES_ROTATION_H_


#include "geometric_primitives/point2D.h"

Point2D rotate(const Point2D& A, double angle);
Point2D rotate(const Point2D& A, const Point2D& O, double angle);

#endif //GEOMETRIC_PRIMITIVES_ROTATION_H_