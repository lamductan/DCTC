#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"


MSTNodeTran::MSTNodeTran(MSTNode* node) {
    MSTNode::init(node);
}

std::string MSTNodeTran::getNodeTypeStr() const {return "MSTNodeTran";}

std::vector<MSTNodeTran*> MSTNodeTran::getMSTChildrenNodes() const {
    return MST_children_nodes_;
}

std::vector<Edge*> MSTNodeTran::getMSTChildrenEdges() const {
    return MST_children_edges_;
}

std::unordered_map<MSTNodeTran*, Edge*> MSTNodeTran::getMSTChildrenEdgesMap() const {
    return MST_children_edges_map_;
}

MSTNodeTran::~MSTNodeTran() {
    //std::cout << getNodeTypeStr() << "'s Destructor\n";
}

std::string MSTNodeTran::getStringContent() const {
    std::string s = MSTNode::getStringContent();
    s += ";node_root_info[";
    if (parent_in_rooted_MST_ != nullptr) {
        s += "par(" + std::to_string(parent_in_rooted_MST_->getId()) + "),";
    }
    s += "children(";
    for(MSTNodeTran* child : MST_children_nodes_) {
        s += std::to_string(child->getId()) + ",";
    }
    s += "),";
    s += "communication_adj_nodes(";
    for(Node* communication_adj_node : getCommunicationEdgeAdjNodes()) {
        s += std::to_string(communication_adj_node->getId()) + ",";
    }
    s += ")";
    s += "]";
    return s;
}