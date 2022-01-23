#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/network_components/nodes/DD_node.h"


class OrientFourNodesPI_2CoverPlaneTest : public ::testing::Test {
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
        std::cout << "Nodes[\n";
        for(const Node* node : nodes)
            std::cout << tab << node->toString() << ",\n";
        std::cout << ']' << last_char;
    }

    void test(const std::vector<Point2D>& points, int convex_hull_correct_size) {
        std::vector<Node*> nodes;
        for(const Point2D& point : points)
            nodes.push_back(new DDNode(point, node_type, r_s, r_c, theta_s, theta_c));
        std::cout << "Before orientation\n";
        print_vector_nodes(nodes, '\n');
        orientFourNodesPI_2CoverPlane(nodes);
        std::cout << "After orientation\n";
        print_vector_nodes(nodes, '\n');
        ASSERT_TRUE(canNodesCoverPlane(nodes));
        clear_vector_nodes(nodes);
    }
};

TEST_F(OrientFourNodesPI_2CoverPlaneTest, TestConvexHull_1)
{
    Point2D K(20, 7);
    Point2D I(15, 4);
    Point2D L(16, 9);
    Point2D J(19, 5);
    std::vector<Point2D> points{K, I, L, J};
    test(points, 4);
}

TEST_F(OrientFourNodesPI_2CoverPlaneTest, TestConvexHull_2)
{
    Point2D P(26, 10);
    Point2D Q(27, 3);
    Point2D N(24, 4);
    Point2D O(30, 2);
    std::vector<Point2D> points{P, Q, N, O};
    test(points, 3);
}

TEST_F(OrientFourNodesPI_2CoverPlaneTest, TestConvexHull_3)
{
    Point2D R(12, 1);
    Point2D U(18, 1);
    Point2D T(16, 1);
    Point2D S(14, 1);
    std::vector<Point2D> points{R, U, T, S};
    test(points, 0);
}