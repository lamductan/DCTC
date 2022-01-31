#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/network_components/nodes/DD_node.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"


class SimpleRelaysAlgTest : public ::testing::Test {
protected:
    long double r_c = 1;
    long double theta_c = PI_2;
    Point2D V = Point2D(0.5, 1);
    Point2D W = Point2D(4.5, 1);
    Point2D Z = Point2D(0.5, 3);
    Point2D A = Point2D(4.5, 0);
    Node* node_V = new DDNode(V.getX(), V.getY(), SENSING_DD_NODE, r_c, theta_c, r_c, theta_c);
    Node* node_W = new DDNode(W.getX(), W.getY(), SENSING_DD_NODE, r_c, theta_c, r_c, theta_c);
    Node* node_Z = new DDNode(Z.getX(), Z.getY(), SENSING_DD_NODE, r_c, theta_c, r_c, theta_c);
    Node* node_A = new DDNode(A.getX(), A.getY(), SENSING_DD_NODE, r_c, theta_c, r_c, theta_c);
    MSTGraph* MST_graph;
    SimpleRelaysAlg* simple_relays_alg;

public:
    void SetUp() override {
        MST_graph = new MSTGraph({node_V, node_W, node_Z, node_A});
        std::cout << "Input graph = " << '\n';
        std::cout << *MST_graph << '\n';
        simple_relays_alg = new SimpleRelaysAlg(MST_graph, r_c, theta_c);
    }

    void TearDown() override {
        delete node_V;
        delete node_W;
        delete MST_graph;
        delete simple_relays_alg;
    }
};

TEST_F(SimpleRelaysAlgTest, TestSimpleRelaysAlg_LongEdges)
{
    for(Edge* edge : MST_graph->getMSTEdges()) {
        if (simple_relays_alg->isLongEdge(edge)) {
            SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_edge_result_LEF =
                simple_relays_alg->steinerizeLongOrMediumEdge(edge);
            std::cout << "Test steinerizing long edges\n"; 
            std::cout << "Type 1 relays:\n";
            print_vector_ptr<Node>(steinerize_long_edge_result_LEF.type_1_relays, '\n');
            std::cout << "Type 2 relays position";
            print_vector_ptr_new_line<Node>(steinerize_long_edge_result_LEF.type_2_relays, '\n');
            std::cout << "Communication Edges";
            print_vector_ptr_new_line<Edge>(steinerize_long_edge_result_LEF.communication_edges, '\n');
        }
    } 
}