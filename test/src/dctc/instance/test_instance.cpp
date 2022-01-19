#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "dctc/instance/instance.h"


TEST(TestInstance, TestInstance_1)
{
    bool deterministic = true;
    int n_targets = 5;
    double min_range = 0;
    double max_range = 10;
    NodeType node_type = SENSING_DD_NODE;
    double r_s = 2.0;
    double r_c = 1.0;
    double theta_s = PI_3;
    double theta_c = PI_2;

    Instance instance(
        n_targets, min_range, max_range,
        node_type, r_s, r_c, theta_s, theta_c,
        deterministic);
    std::vector<Node*> coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
    MSTGraph* MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    std::cout << "MST_graph_ptr:" << '\n';
    std::cout << *MST_graph_ptr << '\n';
    std::cout << "Done MST_graph_ptr" << '\n';

    MSTGraph* MST_graph_copied_ptr = MST_graph_ptr->deepCopy();
    
    delete MST_graph_ptr;

    std::cout << "MST_graph_copied_ptr:" << '\n';
    std::cout << *MST_graph_copied_ptr << '\n';

    delete MST_graph_copied_ptr;
    for(Node* coverage_sensor : coverage_sensors)
        delete coverage_sensor;
}