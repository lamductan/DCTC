#ifndef GEOMETRIC_PRIMITIVES_SEGMENT2D_H_
#define GEOMETRIC_PRIMITIVES_SEGMENT2D_H_

#include <string>
#include <vector>
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"


class Segment2D: public Line2D {
private:
    Point2D endpoint1;
    Point2D endpoint2;
    Segment2D(const Line2D& line2D);

public:
    Segment2D();
    Segment2D(const Point2D& p1, const Point2D& p2);
    static Segment2D fromTwoPoints(const Point2D& p1, const Point2D& p2);
    double length() const;
    Point2D getEndpoint1() const;
    Point2D getEndpoint2() const;
    std::pair<Point2D, Point2D> getEndpoints() const;

    Point2D getMidPoint() const;
    virtual bool passesThroughPoint2D(const Point2D& p) const override;
    virtual bool containsPoint2D(const Point2D& p) const;
    virtual bool isIdentical(const Segment2D& other) const;
    virtual bool intersectsWithLine2D(const Line2D& line2D) const override;
    virtual Point2D findIntersectionsWithLine2D(const Line2D& line2D) const;
    bool intersectsWithSegment2D(const Segment2D& other) const;
    Point2D findIntersectionsWithSegment2D(const Segment2D& other) const;
    Line2D getPerpendicularBisector() const;
    virtual std::string toString() const override;
    virtual std::vector<Point2D> getPointsOnObject(int n_points) const override;
};

#endif //GEOMETRIC_PRIMITIVES_SEGMENT2D_H_