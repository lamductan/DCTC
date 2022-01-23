#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"


class RelaysUtilsTest : public ::testing::Test {
protected:
    double r_c = 1;
    double theta_c = PI_2;
    Point2D V = Point2D(0.5, 1);
    Point2D W = Point2D(4.5, 1);
    Segment2D segment2D = Segment2D(V, W);

public:
};

TEST_F(RelaysUtilsTest, TestRelaysUtils_1)
{
    std::pair<Point2D, Point2D> type_1_relays_pos = calculateRelaysType1Positions(segment2D, r_c);
    std::cout << "Type 1 relays position: ";
    std::cout << type_1_relays_pos.first << ' ' << type_1_relays_pos.second << '\n';
    std::vector<Point2D> type_2_relays_pos = calculateRelaysType2Positions_LongEdge(segment2D, type_1_relays_pos, r_c);
    std::cout << "Type 2 relays position: ";
    print_vector<Point2D>(type_2_relays_pos, '\n');
}

TEST_F(RelaysUtilsTest, TestRelaysUtils_2)
{
    Point2D A = Point2D(1, 1);
    Point2D B = Point2D(3, 1);
    std::pair<Point2D, Point2D> type_1_relays_pos = calculateRelaysType1Positions(Segment2D(A, B), r_c);
    std::cout << "Type 1 relays position: ";
    std::cout << type_1_relays_pos.first << ' ' << type_1_relays_pos.second << '\n';
    std::vector<Point2D> type_2_relays_pos = calculateShortEdgeRelaysPos_TwoNonFree(
        Segment2D(type_1_relays_pos.first, type_1_relays_pos.second));
    std::cout << "Type 2 relays position: ";
    print_vector<Point2D>(type_2_relays_pos, '\n');
}