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


TEST(TestSaveGraph, Test1)
{
    long unsigned int seed = 1;

    int n_targets = 20;
    long double min_range = 0;
    long double max_range = 20;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 5;
    long double r_c = 5;
    long double theta_s = PI_2;
    long double theta_c = PI_2;
    Instance instance;
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    std::string save_dir = PROJECT_ROOT_PATH + "/visualization/samples/";
    Logger logger(save_dir + "results.txt");

    instance = Instance(
        n_targets, min_range, max_range,
        node_type, r_s, r_c, theta_s, theta_c,
        true, seed);
    instance.save(save_dir + "instance.txt");
    coverage_sensors = instance.putCoverageSensors(STRIP_COVERAGE_ALG);
    MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    MST_graph_ptr->save(save_dir + "mst_graph.txt");
    std::cout << "Done MST_graph_ptr" << '\n';
    std::cout << "Done SetUp()\n";

    /* Aschner */
    std::cout << "\n-----------------------------Aschner--------------------------------------------\n";
    MSTGraphAschner* MST_graph_Aschner_ptr = new MSTGraphAschner(MST_graph_ptr);
    MSTGraph* result_MST_graph_Aschner_ptr = MST_graph_Aschner_ptr->doAllSteps();
    SimpleRelaysAlg* simple_relays_alg_Aschner = new SimpleRelaysAlg(result_MST_graph_Aschner_ptr, r_c, theta_c);
    RelaysMSTGraph* relays_MST_graph_Aschner = simple_relays_alg_Aschner->solve();
    ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
    std::cout << "Aschner's beta = " << relays_MST_graph_Aschner->getBeta() << '\n';
    relays_MST_graph_Aschner->save(save_dir + "relays_mst_graph_Aschner.txt");

    /* Tran */
    std::cout << "\n-----------------------------Tran-----------------------------------------------\n";
    MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
    MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
    SimpleRelaysAlg* simple_relays_alg_Tran = new SimpleRelaysAlg(result_MST_graph_Tran_ptr, r_c, theta_c);
    RelaysMSTGraph* relays_MST_graph_Tran = simple_relays_alg_Tran->solve();
    ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
    std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';
    relays_MST_graph_Tran->save(save_dir + "relays_mst_graph_Tran.txt");

    /* Lam_LEF */
    std::cout << "\n-----------------------------Lam_LEF--------------------------------------------\n";
    LongEdgeFirstRelaysAlg* long_edge_first_relays_alg = new LongEdgeFirstRelaysAlg(
        MST_graph_ptr, r_c, theta_c);
    RelaysMSTGraph* relays_MST_graph_Lam_LEF = long_edge_first_relays_alg->solve();
    ASSERT_TRUE(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_LEF));
    std::cout << "Lam_LEF's beta = " << relays_MST_graph_Lam_LEF->getBeta() << '\n';
    relays_MST_graph_Lam_LEF->save(save_dir + "relays_mst_graph_Lam_LEF.txt");

    /* Lam_SEF */
    std::cout << "\n-----------------------------Lam_SEF--------------------------------------------\n";

    /* Write result */
    logger.append("n_total_omni = %d\n", MST_graph_ptr->getNTotalNodesOmni());
    logger.append(
        "Aschner: %d %.3f\n",
        relays_MST_graph_Aschner->getNumTotalNodes(), (double) relays_MST_graph_Aschner->getBeta());
    logger.append(
        "Tran: %d %.3f\n",
        relays_MST_graph_Tran->getNumTotalNodes(), (double) relays_MST_graph_Tran->getBeta());
    logger.append(
        "Lam_LEF: %d %.3f\n",
        relays_MST_graph_Lam_LEF->getNumTotalNodes(), (double) relays_MST_graph_Lam_LEF->getBeta());

    delete MST_graph_ptr;

    delete MST_graph_Aschner_ptr;
    delete result_MST_graph_Aschner_ptr;
    delete simple_relays_alg_Aschner;
    delete relays_MST_graph_Aschner;

    delete MST_graph_Tran_ptr;
    delete result_MST_graph_Tran_ptr;
    delete simple_relays_alg_Tran;
    delete relays_MST_graph_Tran;

    for(Node* coverage_sensor : coverage_sensors)
        delete coverage_sensor;
    std::cout << "Done TearDown\n";
}