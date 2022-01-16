#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "dctc/network_components/nodes/DO_node.h"


TEST(TestDONode, TestDONode_1)
{
    DONode node_1;
    std::cout << node_1 << '\n';
    DONode node_2(1, 2, RELAY_DO_NODE, 5, PI_2, 10);
    std::cout << node_2 << '\n';
}

TEST(TestDONode, TestDONode_2)
{
    DONode node_3(-1, -2, SENSING_DO_NODE, 1, PI_2, 9.3);
    std::cout << node_3 << '\n';
    DONode node_4(1, 2, RELAY_DO_NODE, 5, PI_2, 10);
    std::cout << node_4 << '\n';
}