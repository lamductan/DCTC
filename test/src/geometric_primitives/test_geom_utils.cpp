#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"


TEST(TestGeomUtils, TestGetPointOnRayAtDistance)
{
    Point2D p1(1, 2);
    Ray2D ray2D = Ray2D::fromPointAndDirectionVector(p1, Vector2D(1, -1));
    double distance = sqrt(3);
    Point2D p2 = getPointOnRayAtDistance(ray2D, distance);
    ASSERT_TRUE(ray2D.passesThroughPoint2D(p2));
    ASSERT_DOUBLE_EQ(computeEuclidDistance(p1, p2), distance);
}

TEST(TestGeomUtils, TestGetPointOnSegmentAtDistance)
{
    Point2D p1(1, 2);
    Point2D p2(10.3, sqrt(2));
    Segment2D segment2D = Segment2D::fromTwoPoints(p1, p2);
    double distance = sqrt(5);
    Point2D p3 = getPointOnSegmentAtDistanceFromEndpoint1(segment2D, distance);
    ASSERT_TRUE(segment2D.passesThroughPoint2D(p3));
    ASSERT_DOUBLE_EQ(computeEuclidDistance(p1, p3), distance);
    Point2D p4 = getPointOnSegmentAtDistanceFromEndpoint2(segment2D, distance);
    ASSERT_TRUE(segment2D.passesThroughPoint2D(p4));
    ASSERT_DOUBLE_EQ(computeEuclidDistance(p2, p4), distance);
}

TEST(TestGeomUtils, TestGetLineParallel)
{
    Point2D p1(0, 0);
    Line2D l1(p1, Vector2D(-PI_4, log(3)));
    Point2D p2(1, 2);
    Line2D l2 = getLine2DParallelLineAndPassingThroughPoint(l1, p2);
    ASSERT_TRUE(l2.passesThroughPoint2D(p2) && isLineParallelLine(l1, l2));
}

TEST(TestGeomUtils, TestGetLinePerpendicular)
{
    Point2D p1(0, 0);
    Line2D l1(p1, Vector2D(-PI_4, log(3)));
    Point2D p2(1, 2);
    Line2D l2 = getLine2DPerpendicularLineAndPassingThroughPoint(l1, p2);
    ASSERT_TRUE(l2.passesThroughPoint2D(p2) && isLinePerpendicularLine(l1, l2));
}

TEST(TestGeomUtils, TestBisector)
{
    Point2D O(0, 0);
    Point2D D(2, 4);
    Point2D K(7, -1);
    Ray2D bisector = getBisectorOfAngle(O, D, K);
    std::cout << (Line2D) bisector << '\n';
    Line2D ans = Line2D::fromCoefficients(std::vector<double>{0.99, 0.16, -2.61});
    std::cout << ans << '\n';
    std::cout << ans(O) << ' ' << bisector(O) << '\n';
    std::cout << ans(K) << ' ' << bisector(K) << '\n';
    ASSERT_TRUE(almost_eq(fabs(ans(O)), fabs(bisector(O)), 0.05));
    ASSERT_TRUE(almost_eq(fabs(ans(K)), fabs(bisector(K)), 0.05));
}