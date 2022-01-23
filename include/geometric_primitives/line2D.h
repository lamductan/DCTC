#ifndef GEOMETRIC_PRIMITIVES_LINE2D_H_
#define GEOMETRIC_PRIMITIVES_LINE2D_H_

#include <string>
#include <vector>
#include <iostream>
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/geometric_2D_object.h"
#include "geometric_primitives/vector2D.h"


class Line2D : public Geometric2DObject {
protected:
    std::vector<long double> coefs;
    Point2D point0;
    Vector2D norm_vector;
    int largest_absolute_value_coef_pos;
    void init(const Point2D& point0, const Vector2D& norm_vector);

public:
    static const int dim = 2;
    static int getDim();

    Line2D(const Point2D& point0=Point2D(0, 0), const Vector2D& norm_vector=Vector2D(1, 0));
    Line2D(const Line2D& other);
    static Line2D fromCoefficients(const std::vector<long double>& coefs);
    static Line2D fromTwoPoints(const Point2D& p1, const Point2D& p2);

    std::vector<long double> getCoefs() const;
    Point2D getPoint0() const;
    Vector2D getNormVector() const;
    int getLargestAbsoluteValueCoefPos() const;

    long double operator()(const Point2D& p) const;
    virtual bool passesThroughPoint2D(const Point2D& p) const;
    virtual bool containsPoint2D(const Point2D& p) const override;
    bool hasParallelNormVectorWithLine2D(const Line2D& other) const;
    bool isParallelWithLine2D(const Line2D& other) const;
    virtual bool isIdentical(const Line2D& other) const;
    virtual bool intersectsWithLine2D(const Line2D& other) const;
    virtual Point2D findIntersectionsWithLine2D(const Line2D& other) const;
    long double distanceToPoint2D(const Point2D& point) const;
    long double distanceToLine2D(const Line2D& other) const;
    virtual std::vector<Point2D> getPointsOnObject(int n_points) const;

    virtual std::string toString() const override;
    friend std::ostream& operator<<(std::ostream& os, const Line2D& line2D);
};

#endif //GEOMETRIC_PRIMITIVES_LINE2D_H_
