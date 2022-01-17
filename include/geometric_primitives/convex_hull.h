#ifndef GEOMETRIC_PRIMITIVES_CONVEX_HULL_H_
#define GEOMETRIC_PRIMITIVES_CONVEX_HULL_H_

#include <vector>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"


int orientation(const Point2D& p, const Point2D& q, const Point2D& r);
std::vector<Point2D> findConvexHull(std::vector<Point2D> points);

#endif //GEOMETRIC_PRIMITIVES_CONVEX_HULL_H_