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
    const MSTGraph& MST_graph = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    std::cout << MST_graph << '\n';
    delete &MST_graph;
}