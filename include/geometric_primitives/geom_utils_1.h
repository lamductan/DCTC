#ifndef GEOMETRIC_PRIMITIVES_GEOM_UTILS_1_H_
#define GEOMETRIC_PRIMITIVES_GEOM_UTILS_1_H_

#include <vector>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/circle.h"


double computeCCW(const Point2D& a, const Point2D& b, const Point2D& c);
double computeCCW(const Vector2D& a, const Vector2D& b);
double computeAngle(const Vector2D& a, const Vector2D& b);
double computeAngle(const Point2D& b, const Point2D& a, const Point2D& c); //compute \angle BAC
Ray2D getBisectorOfAngle(const Point2D& b, const Point2D& a, const Point2D& c);

double computeGeometricAngle(const Vector2D& a, const Vector2D& b);
double computeGeometricAngle(const Point2D& b, const Point2D& a, const Point2D& c); //compute \angle BAC
double computeAngle(const Ray2D& r1, const Ray2D& r2);
double computeAngle(const Ray2D& r, const Vector2D& v);
double computeAngle(const Vector2D& v, const Ray2D& r);
double computeAngle(const Line2D& l1, const Line2D& l2);

double computeOppositeAngle(double angle);

Point2D getPointOnRayAtDistance(const Ray2D& ray2D, double distance);
Point2D getPointOnSegmentAtDistanceFromEndpoint1(const Segment2D& segment2D, double distance);
Point2D getPointOnSegmentAtDistanceFromEndpoint2(const Segment2D& segment2D, double distance);

Line2D getLine2DParallelLineAndPassingThroughPoint(const Line2D& line2D, const Point2D& p);
Line2D getLine2DPerpendicularLineAndPassingThroughPoint(const Line2D& line2D, const Point2D& p);
bool isLineParallelLine(const Line2D& l1, const Line2D& l2);
bool isLinePerpendicularLine(const Line2D& l1, const Line2D& l2);

std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Ray2D& ray2D, const Segment2D& segment2D);
std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Segment2D& segment2D, const Ray2D& ray2D);
std::vector<Point2D> findIntersectionsBetweenGeom2DObjects(const Segment2D& segment2D_1, const Segment2D& segment2D_2);


#endif //GEOMETRIC_PRIMITIVES_GEOM_UTILS_1_H_