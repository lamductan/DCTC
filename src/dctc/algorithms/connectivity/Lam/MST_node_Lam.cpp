#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/Lam/MST_node_Lam.h"


MSTNodeLam::MSTNodeLam(MSTNode* node, bool deep_copy) {
    MSTNode::init(node, deep_copy);
}

std::string MSTNodeLam::getNodeTypeStr() const {return "MSTNodeLam";}

MSTNodeLam::~MSTNodeLam() {
    //std::cout << getNodeTypeStr() << "'s Destructor\n";
}

std::string MSTNodeLam::getStringContent() const {
    std::string s = MSTNode::getStringContent();
    s += ";free_=" + std::to_string(free_);
    s += ";long_or_medium_edge_adj_nodes(";
    for(Node* long_or_medium_edge_adj_node : long_or_medium_edge_adj_nodes_) {
        s += std::to_string(long_or_medium_edge_adj_node->getId()) + ",";
    }
    s += ")";
    s += ";short_edge_adj_nodes(";
    for(Node* short_edge_adj_node : short_edge_adj_nodes_) {
        s += std::to_string(short_edge_adj_node->getId()) + ",";
    }
    s += ")";
    return s;
}