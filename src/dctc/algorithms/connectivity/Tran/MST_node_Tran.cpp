#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"


MSTNodeTran::MSTNodeTran(MSTNode* node) {
    MSTNode::init(node);
}

std::string MSTNodeTran::getNodeTypeStr() const {return "MSTNodeTran";}

MSTNodeTran::~MSTNodeTran() {
    //std::cout << getNodeTypeStr() << "'s Destructor\n";
}

std::string MSTNodeTran::getStringContent() const {
    std::string s = MSTNode::getStringContent();
    s += ";communication_adj_nodes(";
    for(Node* communication_adj_node : getCommunicationEdgeAdjNodes()) {
        s += std::to_string(communication_adj_node->getId()) + ",";
    }
    s += ")";
    return s;
}