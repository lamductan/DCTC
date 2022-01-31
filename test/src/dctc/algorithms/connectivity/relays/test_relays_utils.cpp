#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"
#include "random/generator.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"


class RelaysUtilsTest : public ::testing::Test {
protected:
    long double r_c = 1;
    long double theta_c = PI_2;
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

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test1)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 4.1);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 3);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test2)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 3);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 2.8);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test3)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 3);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 4.9);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test4)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, PI_2 + PI_4);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 2.39);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test5)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, PI_2 + PI_4);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 0.63);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test6)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, PI_2 + PI_4);
    Sector sB(Point2D(3, 2), 1.0, PI_2, PI_2 + PI_4);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test7)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 5.7);
    Sector sB(Point2D(3, 2), 1.0, PI_2, 0);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test8)
{
    int n_tests = 1000;
    long unsigned int seed = time(NULL);
    DoubleGenerator generator(0, TWO_PI, false, seed);
    for(int i = 0; i < n_tests; ++i) {
        long double orientation_angle_A = generator.next();
        long double orientation_angle_B = generator.next();
        std::cout << "seed = " << seed << " test " << i << '\n';
        std::cout << "orientation_angle_A = " << orientation_angle_A << '\n';
        std::cout << "orientation_angle_B = " << orientation_angle_B << '\n';
        Sector sA(Point2D(2, 2), 1.0, PI_2, orientation_angle_A);
        Sector sB(Point2D(3, 2), 1.0, PI_2, orientation_angle_B);
        std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
        print_vector<Point2D>(relays_pos, '\n');
    }
}

TEST(RelaysUtilsTestShortEdgeRelays_OneFree, Test1)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 0.63);
    Sector sB(Point2D(3, 2), 1.0, PI_2);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_OneFree(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_OneFree, Test2)
{
    Sector sA(Point2D(2, 2), 1.0, PI_2, 1.63);
    Sector sB(Point2D(3, 2), 1.0, PI_2);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_OneFree(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}

TEST(RelaysUtilsTestShortEdgeRelays_OneFree, Test3)
{
    int n_tests = 1000;
    long unsigned int seed = time(NULL);
    DoubleGenerator generator(0, TWO_PI, false, seed);
    for(int i = 0; i < n_tests; ++i) {
        long double orientation_angle_A = generator.next();
        std::cout << "seed = " << seed << " test " << i << '\n';
        std::cout << "orientation_angle_A = " << orientation_angle_A << '\n';
        Sector sA(Point2D(2, 2), 1.0, PI_2, orientation_angle_A);
        Sector sB(Point2D(3, 2), 1.0, PI_2);
        std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_OneFree(&sA, &sB);
        print_vector<Point2D>(relays_pos, '\n');
    }
}

TEST(RelaysUtilsTestShortEdgeRelays_TwoFixed, Test10)
{
    Sector sA(Point2D(24.8, 81.3), 5, PI_2, 4.27);
    Sector sB(Point2D(28.4, 84.7), 5, PI_2, 0.72);
    std::vector<Point2D> relays_pos = calculateShortEdgeRelaysPos_TwoNonFree_general(&sA, &sB);
    print_vector<Point2D>(relays_pos, '\n');
}