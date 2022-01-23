#ifndef GEOMETRIC_PRIMITIVES_POINT2D_H_
#define GEOMETRIC_PRIMITIVES_POINT2D_H_

#include <cmath>
#include <climits>
#include <iostream>
#include <string>
#include <vector>


class Point2D {
protected:
    long double x;
    long double y;

public:
    static const int dim = 2;
    static int getDim();

    Point2D();
    Point2D(long double x, long double y);
    Point2D(const Point2D& other);
    
    long double getX() const;
    long double getY() const;

    void setX(long double x);
    void setY(long double y);

    Point2D operator+(const Point2D& other) const;
    Point2D operator-(const Point2D& other) const;
    Point2D operator*(long double t) const;
    Point2D operator/(long double t) const;
    Point2D& operator=(const Point2D& other);

    bool operator==(const Point2D& other) const;
    bool operator<(const Point2D& other) const;
    bool operator>(const Point2D& other) const;
    bool operator<=(const Point2D& other) const;
    bool operator>=(const Point2D& other) const;

    long double dot(const Point2D& other) const;
    long double cross(const Point2D& other) const;
    long double computeEuclidDistance(const Point2D& other) const;
    bool isPointInfinity() const;

    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Point2D& point2D);
};

const Point2D POINT_INFINITY = Point2D((long double) LONG_LONG_MAX, (long double) LONG_LONG_MAX);

Point2D operator*(long double t, const Point2D& p);
long double computeEuclidDistance(const Point2D& p1, const Point2D& p2);
std::vector<long double> Point2DToVector(const Point2D& point2D);
Point2D VectorToPoint2D(const std::vector<long double>& v);
bool almost_eq(const Point2D& p1, const Point2D& p2, long double allowed_diff);

#endif //GEOMETRIC_PRIMITIVES_POINT2D_H_