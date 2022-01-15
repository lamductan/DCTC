#include <cmath>
#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/geometric_2D_object.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/ray2D.h"


TEST(TestRay2D, TestRay2D_1)
{
    Point2D point1 = Point2D(1, 1);
    Vector2D direction_vector = Vector2D(2, 2);
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(point1, direction_vector);
    std::cout << ray2D << '\n';
}

TEST(TestRay2D, TestRay2D_intersect_1)
{
    Ray2D r1 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(0, -10));
    Ray2D r2 = Ray2D::fromPointAndDirectionVector(Point2D(1, 5), Vector2D(-9, 0));
    Ray2D r3 = Ray2D::fromPointAndDirectionVector(Point2D(-1, -11), Point2D(-8, 0));
    Ray2D r4 = Ray2D::fromPointAndDirectionVector(Point2D(1, -2), Point2D(-1, 0));
    Ray2D r5 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(0, 10));
    Ray2D r6 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(10, 10));
    Ray2D r7 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(-10, -100));

    ASSERT_FALSE(r1.intersectsWithRay2D(r2));
    ASSERT_FALSE(r1.intersectsWithRay2D(r3));
    ASSERT_TRUE(r1.intersectsWithRay2D(r4));
    ASSERT_TRUE(r1.intersectsWithRay2D(r5));
    ASSERT_TRUE(r1.intersectsWithRay2D(r6));
    ASSERT_TRUE(r1.intersectsWithRay2D(r7));
}

TEST(TestRay2D, TestRay2D_intersect_2)
{
    Ray2D r1 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(0, 10));
    Ray2D r2 = Ray2D::fromPointAndDirectionVector(Point2D(1, 5), Vector2D(9, 0));
    Ray2D r3 = Ray2D::fromPointAndDirectionVector(Point2D(-1, 11), Point2D(8, 0));
    Ray2D r4 = Ray2D::fromPointAndDirectionVector(Point2D(-1, 2), Point2D(1, 0));

    ASSERT_FALSE(r1.intersectsWithRay2D(r2));
    ASSERT_TRUE(r1.intersectsWithRay2D(r3));
    ASSERT_TRUE(r1.intersectsWithRay2D(r4));
}

TEST(TestRay2D, TestRay2D_findIntersection)
{
    Ray2D r1 = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Vector2D(0, 10));
    Ray2D r4 = Ray2D::fromPointAndDirectionVector(Point2D(-1, 2), Vector2D(1, 0));
    ASSERT_EQ(Point2D(0, 2), r1.findIntersectionsWithRay2D(r4));
    std::vector<Ray2D> v{r1, r4};
    print_vector<Ray2D>(v);
    std::cout << '\n';
}

TEST(TestRay2D, TestRay2D_getPointsOnObject)
{
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(Point2D(0, 0), Point2D(10, 10));
    int n_points = 7;
    std::vector<Point2D> v = ray2D.getPointsOnObject(n_points);
    print_vector<Point2D>(v);
    for(Point2D& p : v) {
        ASSERT_TRUE(ray2D.passesThroughPoint2D(p));
    }
}