#ifndef GEOMETRIC_PRIMITIVES_CIRCLE_H_
#define GEOMETRIC_PRIMITIVES_CIRCLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "geometric_primitives/point2D.h"
#include "geometric_primitives/geometric_2D_object.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/segment2D.h"


class Circle : public Geometric2DObject {
protected:
    Point2D center;
    long double radius;
    long double radius_squared;

    void init(const Point2D& center, long double radius);

public:
    Circle();
    Circle(const Point2D& center, long double radius);
    virtual void setRadius(long double radius);
    Point2D getCenter() const;
    long double getRadius() const;
    virtual long double getAngle() const;
    long double operator()(const Point2D& p) const;

    virtual bool passesThroughPoint2D(const Point2D& p) const;
    virtual bool containsPoint2D(const Point2D& p) const override;
    virtual bool containsPoint2DWithInfRange(const Point2D& p) const;
    virtual bool intersectsWithLine2D(const Line2D& line2D) const;
    virtual std::vector<Point2D> findIntersectionsWithLine2D(const Line2D& line2D) const;
    virtual bool intersectsWithRay2D(const Ray2D& ray2D) const;
    virtual std::vector<Point2D> findIntersectionsWithRay2D(const Ray2D& ray2D) const;
    virtual bool intersectsWithSegment2D(const Segment2D& segment2D) const;
    virtual std::vector<Point2D> findIntersectionsWithSegment2D(const Segment2D& segment2D) const;
    virtual bool intersectsWithCircle(const Circle& other) const;
    virtual std::vector<Point2D> findIntersectionsWithCircle(const Circle& other) const;

    virtual std::string toString() const override;
    friend std::ostream& operator<<(std::ostream& os, const Circle& circle);
};

#endif //GEOMETRIC_PRIMITIVES_CIRCLE_H_