#ifndef GEOMETRIC_PRIMITIVES_POINT2D_H_
#define GEOMETRIC_PRIMITIVES_POINT2D_H_

#include <cmath>
#include <climits>
#include <iostream>
#include <string>
#include <vector>


class Point2D {
protected:
    double x;
    double y;

public:
    static const int dim = 2;
    static int getDim();

    Point2D();
    Point2D(double x, double y);
    Point2D(const Point2D& other);
    
    double getX() const;
    double getY() const;

    void setX(double x);
    void setY(double y);

    Point2D operator+(const Point2D& other) const;
    Point2D operator-(const Point2D& other) const;
    Point2D operator*(double t) const;
    Point2D operator/(double t) const;
    Point2D& operator=(const Point2D& other);

    bool operator==(const Point2D& other) const;
    bool operator<(const Point2D& other) const;
    bool operator>(const Point2D& other) const;
    bool operator<=(const Point2D& other) const;
    bool operator>=(const Point2D& other) const;

    double dot(const Point2D& other) const;
    double cross(const Point2D& other) const;
    double computeEuclidDistance(const Point2D& other) const;
    bool isPointInfinity() const;

    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Point2D& point2D);
};

const Point2D POINT_INFINITY = Point2D((double) LONG_LONG_MAX, (double) LONG_LONG_MAX);

Point2D operator*(double t, const Point2D& p);
double computeEuclidDistance(const Point2D& p1, const Point2D& p2);
std::vector<double> Point2DToVector(const Point2D& point2D);
Point2D VectorToPoint2D(const std::vector<double>& v);
bool almost_eq(const Point2D& p1, const Point2D& p2, double allowed_diff);

#endif //GEOMETRIC_PRIMITIVES_POINT2D_H_