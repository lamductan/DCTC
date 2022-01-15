#include <cmath>
#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"


TEST(TestLine2D, TestLine2D_1)
{
    Point2D point0 = Point2D(1, 1);
    Vector2D norm_vector = Vector2D(2, 1);
    Line2D line2D = Line2D(point0, norm_vector);
    std::cout << line2D << '\n';
    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 1, -3});
    std::cout << line2D_1 << '\n';
    Line2D line2D_2 = Line2D::fromTwoPoints(point0, Point2D(0, 3));
    std::cout << line2D_2 << '\n';
}

TEST(TestLine2D, TestLine2D_2)
{
    Point2D point0 = Point2D(1, 1);
    Vector2D norm_vector = Vector2D(0, 2);
    Line2D line2D = Line2D(point0, norm_vector);
    std::cout << line2D << '\n';
    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{0, 1, -1});
    std::cout << line2D_1 << '\n';
    Line2D line2D_2 = Line2D::fromTwoPoints(point0, Point2D(1000, 1));
    std::cout << line2D_2 << '\n';
}

TEST(TestLine2D, TestLine2D_passThrough)
{
    Line2D line2D = Line2D(Point2D(-3, -2), Vector2D(1, 2));
    Point2D p1 = Point2D(3, -5);
    Point2D p2 = Point2D(3, -6);
    ASSERT_TRUE(line2D.passesThroughPoint2D(p1));
    ASSERT_FALSE(line2D.passesThroughPoint2D(p2));

    Line2D line2D_1 = Line2D::fromTwoPoints(Point2D(0, 0), Point2D(0, 1));
    ASSERT_TRUE(line2D_1.passesThroughPoint2D(Point2D(0, 100)));
    ASSERT_FALSE(line2D_1.passesThroughPoint2D(Point2D(1, 1)));
}

TEST(TestLine2D, TestLine2D_identical)
{
    Point2D point0 = Point2D(1, 1);
    Vector2D norm_vector = Vector2D(2, 1);
    Line2D line2D = Line2D(point0, norm_vector);

    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 1, -3});
    ASSERT_TRUE(line2D.isIdentical(line2D_1));

    Line2D line2D_2 = Line2D::fromTwoPoints(point0, Point2D(0, 3));
    ASSERT_TRUE(line2D.isIdentical(line2D_2));
}

TEST(TestLine2D, TestLine2D_parallel)
{
    Point2D point0 = Point2D(1, 1);
    Vector2D norm_vector = Vector2D(2, 1);
    Line2D line2D = Line2D(point0, norm_vector);

    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 1, -3});
    ASSERT_FALSE(line2D.isParallelWithLine2D(line2D_1));

    Line2D line2D_2 = Line2D(Point2D(1, 12), Vector2D(-3, -1.5));
    ASSERT_TRUE(line2D.isParallelWithLine2D(line2D_2));
    ASSERT_TRUE(line2D_2.isParallelWithLine2D(line2D));
}

TEST(TestLine2D, TestLine2D_findIntersection)
{
    Line2D line2D_1 = Line2D(Point2D(100, 0), Vector2D(0, 1));
    Line2D line2D_2 = Line2D(Point2D(0, 10), Vector2D(1, 0));
    Point2D p(0, 0);
    ASSERT_EQ(p, line2D_1.findIntersectionsWithLine2D(line2D_2));

    Line2D line2D_3 = Line2D::fromCoefficients(std::vector<double>{1, 1, -2});
    Line2D line2D_4 = Line2D::fromCoefficients(std::vector<double>{2, 1, -3});
    Point2D p3(1, 1);
    ASSERT_EQ(p3, line2D_3.findIntersectionsWithLine2D(line2D_4));
}

TEST(TestLine2D, TestLine2D_distanceToPoint)
{
    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 3, -13});
    Point2D p1(2, 1);
    ASSERT_DOUBLE_EQ(line2D_1.distanceToPoint2D(p1), 6.0/sqrt(13));
    Line2D line2D_2 = Line2D::fromCoefficients(std::vector<double>{4, 3, -5});
    Point2D p2(2, 4);
    ASSERT_DOUBLE_EQ(line2D_2.distanceToPoint2D(p2), 3);
}

TEST(TestLine2D, TestLine2D_distanceToLine)
{
    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 3, -13});
    Line2D line2D_2 = Line2D::fromCoefficients(std::vector<double>{4, 3, -5});
    ASSERT_TRUE(line2D_1.distanceToLine2D(line2D_2) == 0);
    Line2D line2D_3 = Line2D::fromCoefficients(std::vector<double>{2, 3, -1});
    ASSERT_DOUBLE_EQ(line2D_1.distanceToLine2D(line2D_3), 12.0/sqrt(13));
}

TEST(TestLine2D, TestLine2D_getPointsOnObject)
{
    Line2D line2D_1 = Line2D::fromCoefficients(std::vector<double>{2, 3, -13});
    int n_points = 10;
    std::vector<Point2D> v = line2D_1.getPointsOnObject(n_points);
    print_vector<Point2D>(v);
    std::cout << '\n';
    for(Point2D& p : v) {
        ASSERT_TRUE(line2D_1.passesThroughPoint2D(p));
    }
}