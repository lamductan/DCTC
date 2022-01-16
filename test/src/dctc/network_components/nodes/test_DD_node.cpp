#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "data_structures/counter.h"
#include "dctc/network_components/nodes/DD_node.h"


TEST(TestDDNode, TestDDNode_1)
{
    DDNode node_1;
    std::cout << node_1 << '\n';
    DDNode node_2(1, 2, RELAY_DD_NODE_TYPE_3, 5, PI_2, 10, PI_2);
    std::cout << node_2 << '\n';
}

TEST(TestDDNode, TestDDNode_2)
{
    DDNode node_3(-1, -2, SENSING_DD_NODE, 1, PI_2, 9.3, PI);
    std::cout << node_3 << '\n';
    DDNode node_4(1, 2, RELAY_DD_NODE_TYPE_4, 5, PI_2, 10, PI_2);
    std::cout << node_4 << '\n';
    DDNode node_5 = node_4;
    std::cout << node_5 << '\n';
}

TEST(TestDDNode, TestDDNode_3)
{
    Counter::resetCount();
    DDNode node_3(-1, -2, SENSING_DD_NODE, 1, PI_2, 9.3, PI);
    std::cout << node_3 << '\n';
    Node* node_3_ptr = &node_3;
    Node* node_3_copied_ptr = node_3_ptr->deepCopy();
    std::cout << *node_3_copied_ptr << '\n';
    delete node_3_copied_ptr;
}