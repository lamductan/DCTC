//Usage: ./build/debug data/80.dat -DDEBUG

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


void debug(char* path_cstr) {
    Instance instance = Instance::load(std::string(path_cstr));
    MSTGraph* MST_graph_ptr;
    std::vector<Node*> coverage_sensors; 

    coverage_sensors = instance.putCoverageSensors(STRIP_COVERAGE_ALG);
    MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    std::cout << "Done MST_graph_ptr" << '\n';
    std::cout << "Done SetUp()\n";

    /* Aschner */
    std::cout << "=========================Aschner==============================\n";
    MSTGraphAschner* MST_graph_Aschner_ptr = new MSTGraphAschner(MST_graph_ptr);
    MSTGraph* result_MST_graph_Aschner_ptr = MST_graph_Aschner_ptr->doAllSteps();
    SimpleRelaysAlg* simple_relays_alg_Aschner = new SimpleRelaysAlg(
        result_MST_graph_Aschner_ptr, instance.getRC(), instance.getThetaC());
    RelaysMSTGraph* relays_MST_graph_Aschner = simple_relays_alg_Aschner->solve();
    assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Aschner));
    std::cout << "Aschner's beta = " << relays_MST_graph_Aschner->getBeta() << '\n';
    delete MST_graph_Aschner_ptr;
    delete result_MST_graph_Aschner_ptr;
    delete simple_relays_alg_Aschner;
    delete relays_MST_graph_Aschner;

    /* Tran */
    std::cout << "=========================Tran=================================\n";
    MSTGraphTran* MST_graph_Tran_ptr = new MSTGraphTran(MST_graph_ptr);
    MSTGraph* result_MST_graph_Tran_ptr = MST_graph_Tran_ptr->doAllSteps();
    SimpleRelaysAlg* simple_relays_alg_Tran = new SimpleRelaysAlg(
        result_MST_graph_Aschner_ptr, instance.getRC(), instance.getThetaC());
    RelaysMSTGraph* relays_MST_graph_Tran = simple_relays_alg_Tran->solve();
    assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Tran));
    std::cout << "Tran's beta = " << relays_MST_graph_Tran->getBeta() << '\n';
    delete MST_graph_Tran_ptr;
    delete result_MST_graph_Tran_ptr;
    delete simple_relays_alg_Tran;
    delete relays_MST_graph_Tran;

    /* Lam_LEF */
    std::cout << "=========================Lam_LEF==============================\n";
    LongEdgeFirstRelaysAlg* long_edge_first_relays_alg = new LongEdgeFirstRelaysAlg(
        MST_graph_ptr, instance.getRC(), instance.getThetaC());
    RelaysMSTGraph* relays_MST_graph_Lam_LEF = long_edge_first_relays_alg->solve();
    assert(CommunicationChecker::checkConnectivityAngleAndRange(relays_MST_graph_Lam_LEF));
    std::cout << "Lam_LEF's beta = " << relays_MST_graph_Lam_LEF->getBeta() << '\n';
    delete long_edge_first_relays_alg;
    delete relays_MST_graph_Lam_LEF;

    /* Lam_SEF */
    //TODO: add SEF

    delete MST_graph_ptr;

    for(Node* coverage_sensor : coverage_sensors)
        delete coverage_sensor;
    std::cout << "Done TearDown\n";
}

int main(int argc, char** argv) {
    debug(argv[1]);
    return 0;
}
