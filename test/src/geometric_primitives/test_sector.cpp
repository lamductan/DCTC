#include <cmath>
#include <set>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"


std::set<Point2D> getRoundPoint2DSet(const std::vector<Point2D>& points) {
    std::set<Point2D> st;
    for(const Point2D& point : points) {
        double x = round_double(point.getX());
        double y = round_double(point.getY());
        st.insert(Point2D(x, y));
    }
    return st;
}

bool almost_eq(std::set<Point2D>& s1, std::set<Point2D>& s2, double allowed_diff) {
    if (s1.size() != s2.size()) return false; 
    for(auto it1 = s1.begin(), it2 = s2.begin(); it1 != s1.end() && it2 != s2.end(); ++it1, ++it2) {
        if (!almost_eq(*it1, *it2, allowed_diff)) return false;
    }
    return true;
}

TEST(TestSector, Test_1)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    std::cout << sector << '\n';
    std::cout << sector.getAngle() << ' ' << sector.getOrientationAngle() << '\n';
    std::cout << sector.getMidPoint() << ' ' << sector.getEndpoint1() << ' ' << sector.getEndpoint2() << '\n';
    std::cout << (Line2D) sector.getBoundarySegment1() << '\n';
    std::cout << (Line2D) sector.getBoundarySegment2() << '\n';
    Point2D I(13, 4);
    ASSERT_TRUE(sector.containsPoint2D(I));
}

TEST(TestSector, Test_SectorIntersectRay2D)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Ray2D ray2D = Ray2D(Point2D(15, 6), Point2D(11, 9));
    std::vector<Point2D> intersections = sector.findIntersectionsSectorWithRay2D(ray2D);
    std::set<Point2D> ans{Point2D(11.8, 8.4), Point2D(11.53, 8.6)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_SectorIntersectSegment2D)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Segment2D segment2D(Point2D(9, 9), Point2D(7, 3));
    std::vector<Point2D> intersections = sector.findIntersectionsSectorWithSegment2D(segment2D);
    std::set<Point2D> ans{Point2D(7.8, 5.4), Point2D(8.51, 7.54)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_SectorIntersectSegment2D_1)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Segment2D segment2D(Point2D(9, 9), Point2D(15, 5));
    std::vector<Point2D> intersections = sector.findIntersectionsSectorWithSegment2D(segment2D);
    std::set<Point2D> ans{Point2D(10.27, 8.16), Point2D(12, 7)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_SectorIntersectCircle_1)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Circle circle(Point2D(10.29, 5.86), 2.51);
    std::vector<Point2D> intersections = sector.findIntersectionsSectorWithCircle(circle);
    std::set<Point2D> ans{Point2D(10.42, 3.35), Point2D(8.36, 7.48), Point2D(8.4, 4.2),
                          Point2D(10.76, 8.33), Point2D(7.83, 5.35), Point2D(11.94, 7.75)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_ArcIntersectRay2D)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Ray2D ray2D = Ray2D(Point2D(15, 6), Point2D(11, 9));
    std::vector<Point2D> intersections = sector.findIntersectionsArcWithRay2D(ray2D);
    std::set<Point2D> ans{Point2D(11.8, 8.4)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_ArcIntersectSegment2D)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Segment2D segment2D(Point2D(9, 9), Point2D(7, 3));
    ASSERT_FALSE(sector.arcIntersectsWithSegment2D(segment2D));
}

TEST(TestSector, Test_ArcIntersectSegment2D_1)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Segment2D segment2D(Point2D(9, 9), Point2D(15, 5));
    std::vector<Point2D> intersections = sector.findIntersectionsArcWithSegment2D(segment2D);
    std::set<Point2D> ans{Point2D(12, 7)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_ArcIntersectCircle_1)
{
    Sector sector(Point2D(7, 7), 5.0, 2.0*(PI/6 + 0.2), 5.9);
    Circle circle(Point2D(10.29, 5.86), 2.51);
    std::vector<Point2D> intersections = sector.findIntersectionsArcWithCircle(circle);
    std::set<Point2D> ans{Point2D(11.94, 7.75), Point2D(10.42, 3.35)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_ArcIntersectArc_1)
{
    Sector sector1(Point2D(7, 7), 3.6, 2.0*(PI/6 + 0.4), 5.9);
    Sector sector2(Point2D(15, 7), 4.8, 2.0*(PI/6 + 0.6), 2.6);
    std::vector<Point2D> intersections = sector1.findIntersectionsArcWithArc(sector2);
    std::set<Point2D> ans{Point2D(10.37, 8.27), Point2D(10.37, 5.73)};
    std::set<Point2D> out = getRoundPoint2DSet(intersections);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out, '\n');
    ASSERT_TRUE(almost_eq(ans, out, 0.1));
}

TEST(TestSector, Test_SectorIntersectArc_1)
{
    Sector sector1(Point2D(7, 7), 3.6, 2.0*(PI/6 + 0.4), 5.9);
    Sector sector2(Point2D(13, 7), 4.8, 2.0*(PI/6 + 0.6), 3.4);
    std::vector<Point2D> intersections1 = sector1.findIntersectionsSectorWithArc(sector2);
    std::vector<Point2D> intersections2 = sector2.findIntersectionsArcWithSector(sector1);
    std::set<Point2D> ans{Point2D(8.26, 7.76), Point2D(9.16, 4.12)};
    std::set<Point2D> out1 = getRoundPoint2DSet(intersections1);
    std::set<Point2D> out2 = getRoundPoint2DSet(intersections2);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out1, '\n');
    print_set<Point2D>(out2, '\n');
    ASSERT_TRUE(almost_eq(ans, out1, 0.1));
    ASSERT_TRUE(almost_eq(ans, out2, 0.1));
}

TEST(TestSector, Test_SectorIntersectSector)
{
    Sector sector1(Point2D(7, 7), 3.6, 2.0*(PI/6 + 0.2), 0);
    Sector sector2(Point2D(11, 7), 3.3, 2.0*(PI/6), 3.2);
    std::vector<Point2D> intersections1 = sector1.findIntersectionsSectorWithSector(sector2);
    std::vector<Point2D> intersections2 = sector2.findIntersectionsSectorWithSector(sector1);
    std::set<Point2D> ans{Point2D(7.77, 7.68), Point2D(7.77, 6.32), Point2D(10.59, 7.2),
                          Point2D(10.59, 6.73), Point2D(8.45, 8.28), Point2D(8.71, 5.49)};
    std::set<Point2D> out1 = getRoundPoint2DSet(intersections1);
    std::set<Point2D> out2 = getRoundPoint2DSet(intersections2);
    print_set<Point2D>(ans, '\n');
    print_set<Point2D>(out1, '\n');
    print_set<Point2D>(out2, '\n');
    ASSERT_TRUE(almost_eq(ans, out1, 0.1));
    ASSERT_TRUE(almost_eq(ans, out2, 0.1));
}

TEST(TestSector, Test_OrientToCoverPoint2D_1)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 0.2));
    std::vector<Point2D> points{Point2D(5.33, 9.64), Point2D(3.15, 5.86), Point2D(11.87, 6.89)};
    for(const Point2D& point : points) {
        ASSERT_TRUE(sector.canOrientToCoverPoint2D(point));
        double orientation_angle = sector.orientToCoverPoint2D(point);
        std::cout << orientation_angle << ' ';
        ASSERT_TRUE(sector.containsPoint2D(point));
    }
    std::cout << '\n';
}

TEST(TestSector, Test_OrientToCoverPoint2D_2)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 2));
    std::vector<Point2D> points{Point2D(5.33, 9.64), Point2D(3.15, 5.86), Point2D(11.87, 6.89)};
    for(const Point2D& point : points) {
        ASSERT_TRUE(sector.canOrientToCoverPoint2D(point));
        double orientation_angle = sector.orientToCoverPoint2D(point);
        std::cout << orientation_angle << ' ';
        ASSERT_TRUE(sector.containsPoint2D(point));
    }
    std::cout << '\n';
}

TEST(TestSector, Test_OrientToCoverPoints2D_1)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 2));
    std::vector<Point2D> points{Point2D(5.33, 9.64), Point2D(3.15, 5.86), Point2D(11.68, 7.3)};
    ASSERT_TRUE(sector.canOrientToCoverPoints2D(points));
    double orientation_angle = sector.orientToCoverPoints2D(points);
    std::cout << orientation_angle << '\n';
    ASSERT_TRUE(sector.containsPoints2D(points));
}

TEST(TestSector, Test_OrientToCoverPoints2D_2)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 0.3));
    std::vector<Point2D> points{Point2D(11.48, 7.91), Point2D(8.85, 10.2), Point2D(11.21, 4.71)};
    ASSERT_TRUE(sector.canOrientToCoverPoints2D(points));
    double orientation_angle = sector.orientToCoverPoints2D(points);
    std::cout << orientation_angle << '\n';
    ASSERT_TRUE(sector.containsPoints2D(points));
}

TEST(TestSector, Test_OrientToCoverPoints2D_3)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 0.3));
    std::vector<Point2D> points{Point2D(11.48, 7.91), Point2D(8.4, 10.79), Point2D(11.21, 4.71)};
    ASSERT_FALSE(sector.canOrientToCoverPoints2D(points));
}

TEST(TestSector, Test_OrientBoundary)
{
    Sector sector(Point2D(7, 7), 5, 2.0*(PI/6 + 0.3));
    Point2D a = Point2D(11.48, 7.91);
    Point2D b = Point2D(11.21, 4.71);
    ASSERT_TRUE(sector.orientBoundaryPassingThroughPointAndCoverAnotherPoint(a, b));
    ASSERT_TRUE(sector.getBoundarySegment1().containsPoint2D(a) || sector.getBoundarySegment2().containsPoint2D(a));
    ASSERT_TRUE(sector.containsPoint2D(b));
    double orientation_angle = sector.getOrientationAngle();
    std::cout << orientation_angle << '\n';
    ASSERT_TRUE(orientation_angle >= 5.6 && orientation_angle <= 5.7);
}