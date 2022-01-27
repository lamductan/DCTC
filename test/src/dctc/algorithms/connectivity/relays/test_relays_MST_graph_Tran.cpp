#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "random/generator.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"


class TestRelaysMSTGraphTran : public ::testing::Test {
protected:
    bool deterministic = true;
    long unsigned int seed = 1642737472;

    int n_targets = 200;
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 2.0;
    long double r_c = 50.0;
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
        //long double min_MST_edge_length = MST_graph_ptr->getMinimumMSTEdgeLength();
        //long double max_MST_edge_length = MST_graph_ptr->getMaximumMSTEdgeLength();
        //DoubleGenerator generator(min_MST_edge_length, max_MST_edge_length, true, 1);
        //r_c = generator.next();
        std::cout << "Done MST_graph_ptr" << '\n';
        std::cout << "r_c = " << r_c << '\n';
        std::cout << "Done SetUp()\n";
    }

    void TearDown() override {
        delete MST_graph_ptr;
        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
        std::cout << "Done TearDown\n";
    }
};

TEST_F(TestRelaysMSTGraphTran, Test1)
{
    MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
    MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
    SimpleRelaysAlg* simple_relays_alg_Tran = new SimpleRelaysAlg(result_MST_graph_Tran_ptr, r_c, theta_c);
    RelaysMSTGraph* relays_MST_graph_Tran = simple_relays_alg_Tran->solve();
    ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
    std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';

    delete MST_graph_Tran_ptr;
    delete result_MST_graph_Tran_ptr;
    delete simple_relays_alg_Tran;
    delete relays_MST_graph_Tran;
}

TEST(TestMSTGraphTran1, TestRandomNondeterministic)
{
    int n_tests = 50;
    bool deterministic = false;
    long unsigned int seed = 1;

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

    long double total_beta_Tran = 0.0;

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
        MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
        SimpleRelaysAlg* simple_relays_alg_Tran = new SimpleRelaysAlg(result_MST_graph_Tran_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Tran = simple_relays_alg_Tran->solve();
        ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
        std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';
        total_beta_Tran += relays_MST_graph_Tran->getBeta();


        delete MST_graph_Tran_ptr;
        delete result_MST_graph_Tran_ptr;
        delete simple_relays_alg_Tran;
        delete relays_MST_graph_Tran;

        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
        std::cout << "Done TearDown\n";
    }
    std::cout << "Average Tran's beta = " << total_beta_Tran/n_tests << '\n';
}