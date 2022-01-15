#include <cmath>
#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/vector2D.h"
#include "geometric_primitives/line2D.h"
#include "geometric_primitives/segment2D.h"
#include "geometric_primitives/geom_utils_1.h"


TEST(TestSegment2D, TestSegment2D_1)
{
    Point2D point1 = Point2D(1, 1);
    Point2D point2 = Point2D(2, 2);
    Segment2D segment2D = Segment2D::fromTwoPoints(point1, point2);
    std::cout << segment2D << '\n';
}

TEST(TestSegment2D, TestSegment2D_intersect)
{
    Segment2D s1 = Segment2D::fromTwoPoints(Point2D(0, 0), Point2D(0, 10));
    Segment2D s2 = Segment2D::fromTwoPoints(Point2D(1, 5), Point2D(10, 5));
    Segment2D s3 = Segment2D::fromTwoPoints(Point2D(-1, 11), Point2D(8, 11));
    Segment2D s4 = Segment2D::fromTwoPoints(Point2D(-1, 2), Point2D(8, 2));

    ASSERT_FALSE(s1.intersectsWithSegment2D(s2));
    ASSERT_FALSE(s1.intersectsWithSegment2D(s3));
    ASSERT_TRUE(s1.intersectsWithSegment2D(s4));
}

TEST(TestSegment2D, TestSegment2D_findIntersection)
{
    Segment2D s1 = Segment2D::fromTwoPoints(Point2D(0, 0), Point2D(0, 10));
    Segment2D s4 = Segment2D::fromTwoPoints(Point2D(-1, 2), Point2D(8, 2));
    ASSERT_EQ(Point2D(0, 2), s1.findIntersectionsWithSegment2D(s4));
}

TEST(TestSegment2D, TestSegment2D_getPointsOnObject)
{
    Segment2D segment2D = Segment2D::fromTwoPoints(Point2D(0, 0), Point2D(10, 10));
    int n_points = 7;
    std::vector<Point2D> v = segment2D.getPointsOnObject(n_points);
    print_vector<Point2D>(v);
    std::cout << '\n';
    for(Point2D& p : v) {
        ASSERT_TRUE(segment2D.passesThroughPoint2D(p));
    }
}

TEST(TestSegment2D, TestSegment2D_getPerpendicularBisector)
{
    Segment2D s = Segment2D::fromTwoPoints(Point2D(0, 0), Point2D(0, 10));
    Line2D perpendicular_bisector = s.getPerpendicularBisector();
    ASSERT_TRUE(perpendicular_bisector.passesThroughPoint2D(s.getMidPoint()));
    ASSERT_TRUE(isLinePerpendicularLine(s, perpendicular_bisector));
}