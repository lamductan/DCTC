#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "data_structures/counter.h"
#include "dctc/network_components/nodes/DD_node.h"
#include "dctc/network_components/nodes/DO_node.h"
#include "dctc/network_components/edges/edge.h"


TEST(TestEdge, TestEdge_1)
{
    DDNode node_1(0, 0, SENSING_DD_NODE, 5, PI_2, 10, PI_2);
    DDNode node_2(1, 2, RELAY_DD_NODE_TYPE_3, 6, PI_2, 10, PI_2);
    Edge edge(&node_1, &node_2);
    std::cout << edge << '\n';
}

TEST(TestEdge, TestEdge_2)
{
    DDNode node_1(0, 0, SENSING_DD_NODE, 5, PI_2, 10, PI_2);
    Counter::resetCount(); 
    DDNode node_2(1, 2, RELAY_DD_NODE_TYPE_3, 6, PI_2, 10, PI_2);
    Edge edge(&node_1, &node_2);
    std::cout << edge << '\n';
}