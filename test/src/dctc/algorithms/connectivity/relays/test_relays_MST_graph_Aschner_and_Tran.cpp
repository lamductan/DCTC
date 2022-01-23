#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "random/generator.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Aschner/MST_graph_Aschner.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"


TEST(TestMSTGraphAschnerAndTran1, TestRandomNondeterministic)
{
    int n_tests = 100;
    bool deterministic = false;
    long unsigned int seed = 1;

    int n_targets = 200;
    double min_range = 0;
    double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    double r_s = 50;
    double r_c = 50;
    double theta_s = PI_3;
    double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    double total_beta_Aschner = 0.0;
    double total_beta_Tran = 0.0;

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

        MSTGraphAschner* MST_graph_Aschner_ptr = new MSTGraphAschner(MST_graph_ptr);
        MSTGraph* result_MST_graph_Aschner_ptr = MST_graph_Aschner_ptr->doAllSteps();
        SimpleRelaysAlg* simple_relays_alg_Aschner = new SimpleRelaysAlg(result_MST_graph_Aschner_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Aschner = simple_relays_alg_Aschner->solve();
        ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
        std::cout << "Aschner's beta = " << relays_MST_graph_Aschner->getBeta() << '\n';
        total_beta_Aschner += relays_MST_graph_Aschner->getBeta();

        MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
        MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
        SimpleRelaysAlg* simple_relays_alg_Tran = new SimpleRelaysAlg(result_MST_graph_Tran_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Tran = simple_relays_alg_Tran->solve();
        ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
        std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';
        total_beta_Tran += relays_MST_graph_Tran->getBeta();

        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
        std::cout << "Done TearDown\n";
    }
    std::cout << "Average Aschner's beta = " << total_beta_Aschner/n_tests << '\n';
}