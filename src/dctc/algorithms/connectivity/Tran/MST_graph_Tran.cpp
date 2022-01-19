#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"


MSTGraphTran::MSTGraphTran(const MSTGraph* MST_graph) {
    MSTGraph::cloneFrom(MST_graph, MST_GRAPH_NODE_TRAN);
}

MSTNode* MSTGraphTran::findRootNode() {
    //TODO: implement
    return nullptr;
}

MSTNode* MSTGraphTran::rootTree(MSTNode* root) {
    //TODO: implement
    return nullptr;
}

void MSTGraphTran::initLeavesSet() {
    //TODO: implement
}

void MSTGraphTran::partition() {
    //TODO: implement
}

void MSTGraphTran::establishCommunicationGraph() {
    //TODO: implement
}

void MSTGraphTran::doAllSteps() {
    root = findRootNode();
    rootTree(root);
    partition();
    establishCommunicationGraph();
}

MSTGraphTran::~MSTGraphTran() {
    std::cout << __PRETTY_FUNCTION__ << '\n';
}