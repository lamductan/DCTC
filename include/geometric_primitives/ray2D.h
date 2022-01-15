#ifndef GEOMETRIC_PRIMITIVES_RAY2D_H_
#define GEOMETRIC_PRIMITIVES_RAY2D_H_

#include <string>
#include <vector>
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"


class Ray2D: public Line2D {
private:
    Ray2D(const Line2D& line2D);
    Vector2D direction_vector;

    void init(const Point2D& p, const Vector2D& direction_vector);

public:
    Ray2D();
    Ray2D(const Point2D& p, const Point2D& p1);
    static Ray2D fromPointAndDirectionVector(const Point2D& p, const Vector2D& direction_vector);
    static Ray2D fromTwoPoints(const Point2D& p, const Point2D& p1);
    Vector2D getDirectionVector() const; 
    Vector2D getUnitVector() const;

    bool passesThroughPoint2D(const Point2D& p) const;
    bool containsPoint2D(const Point2D& p) const override;
    bool isIdentical(const Ray2D& other) const;
    bool intersectsWithLine2D(const Line2D& line2D) const;
    Point2D findIntersectionsWithLine2D(const Line2D& line2D) const;
    bool intersectsWithRay2D(const Ray2D& other) const;
    Point2D findIntersectionsWithRay2D(const Ray2D& other) const;
    std::string toString() const override;
    std::vector<Point2D> getPointsOnObject(int n_points) const;
};

#endif //GEOMETRIC_PRIMITIVES_RAY2D_H_