#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"


class TestMSTGraphTran : public ::testing::Test {
protected:
    bool deterministic = true;
    long unsigned int seed = 1642918493;

    int n_targets = 200;
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 50;
    long double r_c = 50;
    long double theta_s = PI_3;
    long double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    void SetUp() override {
        instance = Instance(
            n_targets, min_range, max_range,
            node_type, r_s, r_c, theta_s, theta_c,
            deterministic, seed);
        coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
        MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
        std::cout << "Done MST_graph_ptr" << '\n';
        std::cout << "Done SetUp()\n";
    }

    void TearDown() override {
        delete MST_graph_ptr;
        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
        std::cout << "Done TearDown\n";
    }
};

TEST_F(TestMSTGraphTran, Test1)
{
    MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
    std::cout << "\nMST_graph_Tran_ptr:" << '\n';

    MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
    delete MST_graph_Tran_ptr;

    std::cout << "\nresult_MST_graph_Tran_ptr:" << '\n';
    //std::cout << *result_MST_graph_Tran_ptr << '\n';
    delete result_MST_graph_Tran_ptr;
}

TEST(TestMSTGraphTran1, TestRandomNondeterministic)
{
    int n_tests = 10;
    bool deterministic = false;
    long unsigned int seed = 1;

    int n_targets = 200;
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 2.0;
    long double r_c = 1.0;
    long double theta_s = PI_3;
    long double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    for(int i = 0; i < n_tests; ++i) {
        std::cout << "\nTest " << i << '\n';
        instance = Instance(
            n_targets, min_range, max_range,
            node_type, r_s, r_c, theta_s, theta_c,
            deterministic, seed);
        coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
        MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
        std::cout << "Done MST_graph_ptr" << '\n';
        std::cout << "Done SetUp()\n";

        MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
        std::cout << "MST_graph_Tran_ptr:" << '\n';

        MST_graph_Tran_ptr->doAllSteps();
        delete MST_graph_Tran_ptr;

        delete MST_graph_ptr;
        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
        std::cout << "Done TearDown\n";
    }
}