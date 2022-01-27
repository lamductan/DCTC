#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "utils.h"
#include "random/generator.h"
#include "logger/logger.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Aschner/MST_graph_Aschner.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"


TEST(TestMSTGraphAschnerTranLam_1, TestFixedRS)
{
    int n_tests = 100;
    long unsigned int seed = time(NULL);

    bool deterministic = true;
    if (deterministic) {
        n_tests = 1;
        seed = 1643245398;
    }

    int n_targets = 100; //200
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 50;
    long double theta_s = PI_3;
    long double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    std::string filename = PROJECT_ROOT_PATH + "/results/Aschner_Tran_Lam_1.csv";
    Logger logger(filename);
    logger.write(",Aschner,Tran,Lam_LEF\n");

    for(long double r_c = 10; r_c <= 10; r_c += 10) {
        long double total_beta_Aschner = 0.0;
        long double total_beta_Tran = 0.0;
        long double total_beta_Lam_LEF = 0.0;
        std::cout << "\n===========================================================\n";
        for(int i = 0; i < n_tests; ++i) {
            std::cout << "-----------------------------------------------------------\n";
            std::cout << "Test " << i << ", r_c = " << r_c << '\n';
            instance = Instance(
                n_targets, min_range, max_range,
                node_type, r_s, r_c, theta_s, theta_c,
                deterministic, seed + i);
            coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
            MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
            std::cout << "Done MST_graph_ptr" << '\n';
            std::cout << "Done SetUp()\n";

            /* Aschner */
            std::cout << "=========================Aschner==============================\n";
            MSTGraphAschner* MST_graph_Aschner_ptr = new MSTGraphAschner(MST_graph_ptr);
            MSTGraph* result_MST_graph_Aschner_ptr = MST_graph_Aschner_ptr->doAllSteps();
            SimpleRelaysAlg* simple_relays_alg_Aschner = new SimpleRelaysAlg(result_MST_graph_Aschner_ptr, r_c, theta_c);
            RelaysMSTGraph* relays_MST_graph_Aschner = simple_relays_alg_Aschner->solve();
            ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
            std::cout << "Aschner's beta = " << relays_MST_graph_Aschner->getBeta() << '\n';
            total_beta_Aschner += relays_MST_graph_Aschner->getBeta();
            delete MST_graph_Aschner_ptr;
            delete result_MST_graph_Aschner_ptr;
            delete simple_relays_alg_Aschner;
            delete relays_MST_graph_Aschner;

            /* Tran */
            std::cout << "=========================Tran=================================\n";
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

            /* Lam_LEF */
            std::cout << "=========================Lam_LEF==============================\n";
            LongEdgeFirstRelaysAlg* long_edge_first_relays_alg = new LongEdgeFirstRelaysAlg(
                MST_graph_ptr, r_c, theta_c);
            RelaysMSTGraph* relays_MST_graph_Lam_LEF = long_edge_first_relays_alg->solve();
            ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_LEF));
            std::cout << "Lam_LEF's beta = " << relays_MST_graph_Lam_LEF->getBeta() << '\n';
            total_beta_Lam_LEF += relays_MST_graph_Lam_LEF->getBeta();
            delete long_edge_first_relays_alg;
            delete relays_MST_graph_Lam_LEF;

            /* Lam_SEF */
            //TODO: add SEF

            delete MST_graph_ptr;

            for(Node* coverage_sensor : coverage_sensors)
                delete coverage_sensor;
            std::cout << "Done TearDown\n";
        }

        long double average_beta_Aschner = total_beta_Aschner/n_tests;
        long double average_beta_Tran = total_beta_Tran/n_tests;
        long double average_beta_Lam_LEF = total_beta_Lam_LEF/n_tests;
        std::cout << "Average Aschner's beta = " << average_beta_Aschner << '\n';
        std::cout << "Average Tran's beta    = " << average_beta_Tran << '\n';
        std::cout << "Average Lam_LEF's beta    = " << average_beta_Lam_LEF << '\n';
        logger.append("%d,%.3f,%.3f,%.3f\n", (int) r_c, 
            (double) average_beta_Aschner, (double) average_beta_Tran, (double) average_beta_Lam_LEF);
    }
}