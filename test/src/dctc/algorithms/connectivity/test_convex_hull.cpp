#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/network_components/nodes/DD_node.h"


class ConvexHullTest : public ::testing::Test {
protected:
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 100;
    long double r_c = 100;
    long double theta_s = PI_3;
    long double theta_c = PI_2;

public:
    static void clear_vector_nodes(const std::vector<Node*>& nodes) {
        for(Node* node : nodes) delete node;
    }

    static void print_vector_nodes(const std::vector<Node*>& nodes, char last_char=0) {
        std::string tab = "    "; 
        std::cout << "Nodes[";
        for(const Node* node : nodes)
            std::cout << tab << node->toString() << ",\n";
        std::cout << ']' << last_char;
    }

    void test(const std::vector<Point2D>& points, int convex_hull_correct_size) {
        std::vector<Node*> nodes;
        for(const Point2D& point : points)
            nodes.push_back(new DDNode(point, node_type, r_s, r_c, theta_s, theta_c));
        std::vector<Node*> convex_hull = findConvexHull(nodes);
        print_vector_nodes(convex_hull, '\n');
        ASSERT_EQ(convex_hull.size(), convex_hull_correct_size);
        clear_vector_nodes(convex_hull);
    }
};

TEST_F(ConvexHullTest, TestConvexHull_1)
{
    Point2D K(20, 7);
    Point2D I(15, 4);
    Point2D L(16, 9);
    Point2D J(19, 4);
    std::vector<Point2D> points{K, I, L, J};
    test(points, 4);
}

TEST_F(ConvexHullTest, TestConvexHull_2)
{
    Point2D P(26, 10);
    Point2D Q(27, 4);
    Point2D N(24, 4);
    Point2D O(29, 4);
    std::vector<Point2D> points{P, Q, N, O};
    test(points, 3);
}

TEST_F(ConvexHullTest, TestConvexHull_3)
{
    Point2D R(12, 1);
    Point2D U(18, 1);
    Point2D T(16, 1);
    Point2D S(14, 1);
    std::vector<Point2D> points{R, U, T, S};
    test(points, 0);
}

TEST_F(ConvexHullTest, TestConvexHull_4)
{
    Point2D A(7, 7);
    Point2D B(11.33, 4.5);
    Point2D C(9.16, 11.51);
    Point2D D(9.3, 8.55);
    Point2D G(10.61, 5.73);
    Point2D H(11.48, 7.91);
    std::vector<Point2D> points{A, B, C, D, G, H};
    test(points, 4);
}