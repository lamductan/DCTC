//Usage: ./build/prog 50 100

#include <cassert>
#include <iostream>
#include <string>

#include "utils.h"
#include "random/generator.h"
#include "logger/logger.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/connectivity/Aschner/MST_graph_Aschner.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"


void test1() {
    Point2D A(0, 0);
    Point2D B(5, 0);
    Point2D C(4, 3);
    Point2D D(2, 4);
    Point2D E(-1, 4);
    Point2D F(-2, -1);
    Point2D G(-4, 0);
    Point2D H(0, -4);
    Point2D I(2, -3);
    Point2D J(4, -2);
    Point2D K(7, -1);

    std::cout << "BAX" << '\n';
    std::vector<Point2D> points1{C, D, E, F, G, H, I, J, K};
    std::vector<char> point_names1{'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    for(int i = 0; i < points1.size(); ++i) {
        long double angle = computeAngle(B, A, points1[i]);
        long double geometric_angle = computeGeometricAngle(B, A, points1[i]);
        std::cout << "angle BA" << point_names1[i] << " = " 
            << rad2deg(angle) << ' ' << rad2deg(geometric_angle) << ' ' << computeCCW(B, A, points1[i]) << '\n';
    }

    std::cout << "=========================" << '\n';
    std::cout << "GAX" << '\n';
    std::vector<Point2D> points2{E, D, C, B, K, J, I, H, F};
    std::vector<char> point_names2{'E', 'D', 'C', 'B', 'K', 'J', 'I', 'H', 'F'};
    for(int i = 0; i < points2.size(); ++i) {
        long double angle = computeAngle(G, A, points2[i]);
        long double geometric_angle = computeGeometricAngle(G, A, points2[i]);
        std::cout << "angle GA" << point_names2[i] << " = "
            << rad2deg(angle) << ' ' << rad2deg(geometric_angle) << ' ' << computeCCW(G, A, points2[i]) << '\n';
    }
}

void test2() {
    std::vector<Point2D*> p;
    Vector2D v1(1, 1);
    Vector2D v2(0, 0);
    p.push_back(&v1);
    p.push_back(&v2);
    for(auto x : p) std::cout << *x << ' ';
    std::cout << '\n';
}

void test3() {
    Sector sector(Point2D(1, 1), 5, PI_2, PI_3);
    std::cout << sector << '\n';
}

void test4() {
    bool deterministic = true;
    int n_targets = 5;
    long double min_range = 0;
    long double max_range = 10;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 2.0;
    long double r_c = 1.0;
    long double theta_s = PI_3;
    long double theta_c = PI_2;

    Instance instance(
        n_targets, min_range, max_range,
        node_type, r_s, r_c, theta_s, theta_c,
        deterministic);
    std::vector<Node*> coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
    MSTGraph* MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    std::cout << *MST_graph_ptr << '\n';
    delete MST_graph_ptr;
}

void test5(long double r_c, int n_tests) {
    long unsigned int seed = time(NULL);

    bool deterministic = false;
    if (deterministic) {
        n_tests = 1;
        seed = 1643052521;
    }

    int n_targets = 200;
    long double min_range = 0;
    long double max_range = 1000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 50;
    long double theta_s = PI_3;
    long double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    std::string filename = PROJECT_ROOT_PATH + "/results/Aschner_Tran_Lam_" + std::to_string((int) r_c) + ".csv";
    Logger logger(filename);
    logger.write(",Aschner,Tran,Lam_LEF\n");

    std::string instance_save_path = PROJECT_ROOT_PATH + "/data/" + std::to_string((int) r_c) + ".dat";

    long double total_beta_Aschner = 0.0;
    long double total_beta_Tran = 0.0;
    long double total_beta_Lam_LEF = 0.0;
    std::cout << "\n===========================================================\n";
    for(int i = 0; i < n_tests; ++i) {
        std::cout << "-----------------------------------------------------------\n";
        std::cout << "Test " << i << ", r_c = " << r_c << '\n';
        std::cout << "instance_save_path = " << instance_save_path << '\n';
        instance = Instance(
            n_targets, min_range, max_range,
            node_type, r_s, r_c, theta_s, theta_c,
            deterministic, seed + i);
        instance.save(instance_save_path);
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
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
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
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
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
        assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_LEF));
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

int main(int argc, char** argv) {
    long double r_c = atof(argv[1]);
    int n_tests = atoi(argv[2]);
    test5(r_c, n_tests);
    return 0;
}
