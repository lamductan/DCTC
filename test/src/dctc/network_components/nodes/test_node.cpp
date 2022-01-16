#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "dctc/network_components/nodes/node.h"


TEST(TestNode, TestNode_1)
{
    Node node_1;
    std::cout << node_1 << '\n';
    Node node_2(1, 2, RELAY_DD_NODE_TYPE_1);
    std::cout << node_2 << '\n';
}

TEST(TestNode, TestNode_2)
{
    Node node_3(3, 4, SENSING_DO_NODE);
    std::cout << node_3 << '\n';
    Node node_4(5, 6, RELAY_DD_NODE_TYPE_2);
    std::cout << node_4 << '\n';
    std::cout << node_4.getId() << '\n';
}