#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/ray2D.h"
#include "geometric_primitives/circle.h"


TEST(TestCircle, TestCircle_1)
{
    Circle circle(Point2D(1, 2), sqrtl(5));
    std::cout << circle << '\n';
    Point2D p1(3, 3);
    ASSERT_TRUE(circle.passesThroughPoint2D(p1));
    Point2D p2(2, 3);
    ASSERT_TRUE(circle.containsPoint2D(p2));
    ASSERT_TRUE(circle.containsPoint2D(circle.getCenter()));
    ASSERT_FALSE(circle.passesThroughPoint2D(circle.getCenter()));
}

TEST(TestCircle, TestCircle_IntersectionWithLine2D_1)
{
    Line2D line2D = Line2D::fromCoefficients(std::vector<long double>{1, -2, 3});
    Circle circle(Point2D(1, 2), sqrtl(5));
    ASSERT_TRUE(circle.intersectsWithLine2D(line2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithLine2D(line2D);
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0], Point2D(-1, 1));
    ASSERT_EQ(intersections[1], Point2D(3, 3));
}

TEST(TestCircle, TestCircle_IntersectionWithLine2D_2)
{
    Line2D line2D = Line2D::fromCoefficients(std::vector<long double>{1, 0, -4});
    Circle circle(Point2D(1, 2), sqrtl(5));
    ASSERT_FALSE(circle.intersectsWithLine2D(line2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithLine2D(line2D);
    ASSERT_EQ(intersections.size(), 0);
}

TEST(TestCircle, TestCircle_IntersectionWithLine2D_3)
{
    Line2D line2D = Line2D::fromCoefficients(std::vector<long double>{1, 0, -1});
    Circle circle(Point2D(0, 0), sqrtl(1));
    ASSERT_TRUE(circle.intersectsWithLine2D(line2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithLine2D(line2D);
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0], Point2D(1, 0));
}

TEST(TestCircle, TestCircle_IntersectionWithSegment2D_1)
{
    Segment2D segment2D = Segment2D::fromTwoPoints(Point2D(-0.5, 0), Point2D(0.5, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_FALSE(circle.intersectsWithSegment2D(segment2D));
}

TEST(TestCircle, TestCircle_IntersectionWithSegment2D_2)
{
    Segment2D segment2D = Segment2D::fromTwoPoints(Point2D(-0.5, 0), Point2D(1.5, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_TRUE(circle.intersectsWithSegment2D(segment2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithSegment2D(segment2D);
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0], Point2D(1, 0));
}

TEST(TestCircle, TestCircle_IntersectionWithSegment2D_3)
{
    Segment2D segment2D = Segment2D::fromTwoPoints(Point2D(-1.0, 0), Point2D(1.0, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_TRUE(circle.intersectsWithSegment2D(segment2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithSegment2D(segment2D);
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0], Point2D(-1, 0));
    ASSERT_EQ(intersections[1], Point2D(1, 0));
}

TEST(TestCircle, TestCircle_IntersectionWithRay2D_1)
{
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(Point2D(-0.5, 2), Vector2D(0.5, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_FALSE(circle.intersectsWithRay2D(ray2D));
}

TEST(TestCircle, TestCircle_IntersectionWithRay2D_2)
{
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(Point2D(-0.5, 0), Vector2D(0.5, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_TRUE(circle.intersectsWithRay2D(ray2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithRay2D(ray2D);
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0], Point2D(1, 0));
}

TEST(TestCircle, TestCircle_IntersectionWithRay2D_3)
{
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(Point2D(-1, 0), Vector2D(0.5, 0));
    Circle circle(Point2D(0, 0), 1);
    ASSERT_TRUE(circle.intersectsWithRay2D(ray2D));
    std::vector<Point2D> intersections = circle.findIntersectionsWithRay2D(ray2D);
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0], Point2D(-1, 0));
    ASSERT_EQ(intersections[1], Point2D(1, 0));
}

TEST(TestCircle, TestCircle_IntersectionWithCircle_1)
{
    Circle circle1(Point2D(0, 0), 2);
    Circle circle2(Point2D(5, 5), 2);
    //print_vector<Circle>({circle1, circle2});
    ASSERT_FALSE(circle1.intersectsWithCircle(circle2));
}

TEST(TestCircle, TestCircle_IntersectionWithCircle_2)
{
    Circle circle1(Point2D(0, 0), 2);
    Circle circle2(Point2D(0, 4), 2);
    ASSERT_TRUE(circle1.intersectsWithCircle(circle2));
    std::vector<Point2D> intersections = circle1.findIntersectionsWithCircle(circle2);
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0], Point2D(0, 2));
}

TEST(TestCircle, TestCircle_IntersectionWithCircle_3)
{
    Circle circle1(Point2D(0, 0), 2);
    Circle circle2(Point2D(2, 2), 2);
    ASSERT_TRUE(circle1.intersectsWithCircle(circle2));
    std::vector<Point2D> intersections = circle1.findIntersectionsWithCircle(circle2);
    ASSERT_EQ(intersections[0], Point2D(0, 2));
    ASSERT_EQ(intersections[1], Point2D(2, 0));
}