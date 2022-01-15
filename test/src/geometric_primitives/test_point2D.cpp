#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"


TEST(TestPoint2D, TestPoint2D_1)
{
    Point2D p = Point2D(1, 3);
    std::cout << p.toString() << std::endl;
    ASSERT_TRUE(true);
}

TEST(TestPoint2D, TestPoint2D_Add)
{
    Point2D p1 = Point2D(1, 3);
    Point2D p2 = Point2D(2, 4);
    Point2D p3 = Point2D(3, 7);
    Point2D p = p1 + p2;
    ASSERT_TRUE(approx(p.getX(), p3.getX()) && approx(p.getY(), p3.getY()));
}

TEST(TestPoint2D, TestPoint2D_Subtract)
{
    Point2D p1 = Point2D(1, 3);
    Point2D p2 = Point2D(2, 4);
    Point2D p3 = Point2D(-1, -1);
    Point2D p = p1 - p2;
    ASSERT_TRUE(approx(p.getX(), p3.getX()) && approx(p.getY(), p3.getY()));
}

TEST(TestPoint2D, TestPoint2D_Multiply1)
{
    Point2D p1 = Point2D(1, 3);
    double t = 2;
    Point2D p2 = Point2D(2, 6);
    Point2D p = p1 * t;
    ASSERT_TRUE(approx(p.getX(), p2.getX()) && approx(p.getY(), p2.getY()));
}

TEST(TestPoint2D, TestPoint2D_Multiply2)
{
    Point2D p1 = Point2D(1, 3);
    double t = 2;
    Point2D p2 = Point2D(2, 6);
    Point2D p = t * p1;
    ASSERT_TRUE(approx(p.getX(), p2.getX()) && approx(p.getY(), p2.getY()));
}

TEST(TestPoint2D, TestPoint2D_Division)
{
    Point2D p1 = Point2D(2, 6);
    double t = 2;
    Point2D p2 = Point2D(1, 3);
    Point2D p = p1 / t;
    ASSERT_TRUE(approx(p.getX(), p2.getX()) && approx(p.getY(), p2.getY()));
}

TEST(TestPoint2D, TestPoint2D_Dot)
{
    Point2D p1 = Point2D(2, 6);
    Point2D p2 = Point2D(1, 3);
    double t = p1.dot(p2);
    ASSERT_DOUBLE_EQ(t, 20);
}

TEST(TestPoint2D, TestPoint2D_Cross)
{
    Point2D p1 = Point2D(2, 6);
    Point2D p2 = Point2D(1, 3);
    double t = p1.cross(p2);
    ASSERT_DOUBLE_EQ(t, 0);
}

TEST(TestPoint2D, TestPoint2D_Distance)
{
    Point2D p1 = Point2D(0, 3);
    Point2D p2 = Point2D(-4, 0);
    double dis = p1.computeEuclidDistance(p2);
    ASSERT_DOUBLE_EQ(dis, 5.0);
}