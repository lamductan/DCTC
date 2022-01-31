//Usage: ./build/main2

#include <cassert>
#include <iostream>
#include <string>
#include <cstring>

#include "utils.h"
#include "random/generator.h"
#include "logger/logger.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Aschner/MST_graph_Aschner.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/short_edge_first_relays_alg.h"


long double degToRad1(int deg) {
    if (deg == 60) return PI_3;
    else if (deg == 90) return PI_2;
    return TWO_PI_3;
}

void test6(long double theta_s, long double theta_c, long double r_s, long double r_c,
           int n_tests, std::string out_path, std::string out_filename, long unsigned int timestamp
) {
    long unsigned int seed = timestamp;
    bool deterministic = false;

    int n_targets = 200;
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;

    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    Logger logger(out_path);
    logger.write(",beta_Aschner,beta_Tran,beta_Lam_LEF,n_total_omni,n_total_Aschner,n_total_Tran,n_total_Lam_LEF\n");
    std::string instance_save_path = PROJECT_ROOT_PATH + "/data/" + out_filename + ".dat";

    long double total_beta_Aschner = 0.0;
    long double total_beta_Tran = 0.0;
    long double total_beta_Lam_LEF = 0.0;
    long double total_beta_Lam_SEF = 0.0;
    int n_total_nodes_omni = 0;
    int n_total_nodes_Aschner = 0;
    int n_total_nodes_Tran = 0;
    int n_total_nodes_Lam_LEF = 0;
    int n_total_nodes_Lam_SEF = 0;

    std::cout << "\n===========================================================\n";
    for(int i = 0; i < n_tests; ++i) {
        std::cout << "-----------------------------------------------------------\n";
        std::cout << "Test " << i << ", r_c = " << r_c << '\n';
        instance = Instance(
            n_targets, min_range, max_range,
            node_type, r_s, r_c, theta_s, theta_c,
            deterministic, seed + i);
        instance.save(instance_save_path);
        coverage_sensors = instance.putCoverageSensors(STRIP_COVERAGE_ALG);
        MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
        n_total_nodes_omni += MST_graph_ptr->getNTotalNodesOmni();
        std::cout << "Instance save path = " << instance_save_path << '\n';
        std::cout << "Done MST_graph_ptr" << '\n';
        std::cout << "Done SetUp()\n";

        /* Aschner */
        std::cout << "=========================Aschner==============================\n";
        MSTGraphAschner* MST_graph_Aschner_ptr = new MSTGraphAschner(MST_graph_ptr);
        MSTGraph* result_MST_graph_Aschner_ptr = MST_graph_Aschner_ptr->doAllSteps();
        SimpleRelaysAlg* simple_relays_alg_Aschner = new SimpleRelaysAlg(
            result_MST_graph_Aschner_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Aschner = simple_relays_alg_Aschner->solve();
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
        std::cout << "Aschner's beta = " << relays_MST_graph_Aschner->getBeta() << '\n';
        total_beta_Aschner += relays_MST_graph_Aschner->getBeta();
        n_total_nodes_Aschner += relays_MST_graph_Aschner->getNNodes();
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
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
        std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';
        total_beta_Tran += relays_MST_graph_Tran->getBeta();
        n_total_nodes_Tran += relays_MST_graph_Tran->getNNodes();
        delete MST_graph_Tran_ptr;
        delete result_MST_graph_Tran_ptr;
        delete simple_relays_alg_Tran;
        delete relays_MST_graph_Tran;

        /* Lam_LEF */
        std::cout << "=========================Lam_LEF==============================\n";
        LongEdgeFirstRelaysAlg* long_edge_first_relays_alg = new LongEdgeFirstRelaysAlg(
            MST_graph_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Lam_LEF = long_edge_first_relays_alg->solve();
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_LEF));
        std::cout << "Lam_LEF's beta = " << relays_MST_graph_Lam_LEF->getBeta() << '\n';
        total_beta_Lam_LEF += relays_MST_graph_Lam_LEF->getBeta();
        n_total_nodes_Lam_LEF += relays_MST_graph_Lam_LEF->getNNodes();
        delete long_edge_first_relays_alg;
        delete relays_MST_graph_Lam_LEF;

        /* Lam_SEF */
        std::cout << "=========================Lam_SEF==============================\n";
        ShortEdgeFirstRelaysAlg* short_edge_first_relays_alg = new ShortEdgeFirstRelaysAlg(
            MST_graph_ptr, r_c, theta_c);
        RelaysMSTGraph* relays_MST_graph_Lam_SEF = short_edge_first_relays_alg->solve();
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_SEF));
        std::cout << "Lam_SEF's beta = " << relays_MST_graph_Lam_SEF->getBeta() << '\n';
        total_beta_Lam_SEF += relays_MST_graph_Lam_SEF->getBeta();
        n_total_nodes_Lam_SEF += relays_MST_graph_Lam_SEF->getNNodes();
        delete short_edge_first_relays_alg;
        delete relays_MST_graph_Lam_SEF;

        delete MST_graph_ptr;
        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
    }

    /* Analyzing and logging results */
    long double average_beta_Aschner = total_beta_Aschner/n_tests;
    long double average_beta_Tran = total_beta_Tran/n_tests;
    long double average_beta_Lam_LEF = total_beta_Lam_LEF/n_tests;
    long double average_beta_Lam_SEF = total_beta_Lam_SEF/n_tests;
    double average_n_total_nodes_omni = n_total_nodes_omni/n_tests;
    double average_n_total_nodes_Aschner = n_total_nodes_Aschner/n_tests;
    double average_n_total_nodes_Tran = n_total_nodes_Tran/n_tests;
    double average_n_total_nodes_Lam_LEF = n_total_nodes_Lam_LEF/n_tests;
    double average_n_total_nodes_Lam_SEF = n_total_nodes_Lam_SEF/n_tests;

    std::cout << "Average Aschner's beta = " << average_beta_Aschner << '\n';
    std::cout << "Average Tran's beta    = " << average_beta_Tran << '\n';
    std::cout << "Average Lam_LEF's beta    = " << average_beta_Lam_LEF << '\n';
    std::cout << "Average Lam_SEF's beta    = " << average_beta_Lam_SEF << '\n';
    std::cout << "Average total omni nodes = " << average_n_total_nodes_omni << '\n';
    std::cout << "Average total nodes by Aschner = " << average_n_total_nodes_Aschner << '\n';
    std::cout << "Average total nodes by Tran = " << average_n_total_nodes_Tran << '\n';
    std::cout << "Average total nodes by Lam_LEF = " << average_n_total_nodes_Lam_LEF << '\n';
    std::cout << "Average total nodes by Lam_SEF = " << average_n_total_nodes_Lam_SEF << '\n';

    logger.append("%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n", (int) r_c, 
        (double) average_beta_Aschner, (double) average_beta_Tran, 
        (double) average_beta_Lam_LEF, (double) average_beta_Lam_SEF,
        average_n_total_nodes_omni, average_n_total_nodes_Aschner, average_n_total_nodes_Tran,
        average_n_total_nodes_Lam_LEF, average_n_total_nodes_Lam_SEF);
}

void testFixed_RS(int theta_s_deg, int theta_c_deg, int r_s, int r_c, int n_tests, long unsigned int timestamp) {
    char out_filename[1000];
    snprintf(out_filename, 1000, "%ld-fixed_rs-ts_%03d-tc_%03d-rs_%03d-rc_%03d",
             timestamp, theta_s_deg, theta_c_deg, r_s, r_c);
    std::string out_filename_str(out_filename);
    std::cout << out_filename << '\n';
    long double theta_s = degToRad1(theta_s_deg);
    long double theta_c = degToRad1(theta_c_deg);
    std::string out_path_str = PROJECT_ROOT_PATH + "/results_1/" + out_filename_str + ".csv";
    test6(theta_s, theta_c, r_s, r_c, n_tests, out_path_str, out_filename_str, timestamp);
}

void testFixed_RC(int theta_s_deg, int theta_c_deg, int r_s, int r_c, int n_tests, long unsigned int timestamp) {
    char out_filename[1000];
    snprintf(out_filename, 1000, "%ld-fixed_rc-ts_%03d-tc_%03d-rs_%03d-rc_%03d",
             timestamp, theta_s_deg, theta_c_deg, r_s, r_c);
    std::string out_filename_str(out_filename);
    std::cout << out_filename << '\n';
    long double theta_s = degToRad1(theta_s_deg);
    long double theta_c = degToRad1(theta_c_deg);
    std::string out_path_str = PROJECT_ROOT_PATH + "/results_1/" + out_filename_str + ".csv";
    test6(theta_s, theta_c, r_s, r_c, n_tests, out_path_str, out_filename_str, timestamp);
}

void testFixed_RS1(int n_tests) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
    int theta_c_deg = 90;
    int r_s = 50;
    long unsigned int timestamp = time(NULL);

    for(int theta_s_deg : {60, 90, 120}) {
        for(int r_c = 10; r_c <= 100; r_c += 10) {
            testFixed_RS(theta_s_deg, theta_c_deg, r_s, r_c, n_tests, timestamp);
        }
    }
} 

void testFixed_RC1(int n_tests) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
    int theta_c_deg = 90;
    int r_c = 70;
    long unsigned int timestamp = time(NULL);

    for(int theta_s_deg : {60, 90, 120}) {
        for(int r_s = 10; r_s <= 100; r_s += 10) {
            testFixed_RC(theta_s_deg, theta_c_deg, r_s, r_c, n_tests, timestamp);
        }
    }
} 

int main(int argc, char** argv) {
    int n_tests = 2;
    if (argc == 2) {
        n_tests = atoi(argv[1]);
    }
    testFixed_RS1(n_tests);
    testFixed_RC1(n_tests);
    return 0;
}
