#include <cassert>
#include <iostream>

#include "dctc/utils.h"
#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/nodes/MST_node.h"


MSTNode::MSTNode() {}

void MSTNode::init(Node* node) {
    Node::init(node->getX(), node->getY(), node->getNodeType());
    id_ = node->getId();
    node_ = node->deepCopy(); //Create new object. Should be delete later
}

void MSTNode::init(MSTNode* node) {
    init((Node*) node);
    is_leaf_ = node->is_leaf_;
}

MSTNode::MSTNode(Node* node) {
    init(node);
}

MSTNode::MSTNode(const Point2D& point2D, NodeType node_type) {
    Node node(point2D, node_type);
    init(&node);
}

MSTNode::MSTNode(MSTNode* node) {
    init(node);
}

std::string MSTNode::getNodeTypeStr() const {return "MSTNode";}

Circle* MSTNode::getSensingSector() const {return node_->getSensingSector();}

Circle* MSTNode::getCommunicationAntenna() const {return node_->getCommunicationAntenna();}

bool MSTNode::isLeaf() const {return is_leaf_;}

void MSTNode::setIsLeaf(bool is_leaf) {is_leaf_ = is_leaf;}

void MSTNode::addMSTEdge(Edge* edge) {
    mst_edges_[(MSTNode*) edge->getOtherEndpoint(this)] = edge;
}

std::unordered_map<MSTNode*, Edge*> MSTNode::getMSTEdgesMap() const {return mst_edges_;}

std::vector<MSTNode*> MSTNode::getMSTEdgeAdjNodes() const {
    return getMapKeys<MSTNode*, Edge*>(mst_edges_);
}

std::vector<Edge*> MSTNode::getMSTEdges() const {
    return getMapValues<MSTNode*, Edge*>(mst_edges_);
}

void MSTNode::addCommunicationEdge(Edge* edge) {
    communication_edges_[edge->getOtherEndpoint(this)] = edge;
}

std::unordered_map<Node*, Edge*> MSTNode::getCommunicationEdgesMap() const {return communication_edges_;}

std::vector<Node*> MSTNode::getCommunicationEdgeAdjNodes() const {
    return getMapKeys<Node*, Edge*>(communication_edges_);
}

std::vector<Edge*> MSTNode::getCommunicationEdges() const {
    return getMapValues<Node*, Edge*>(communication_edges_);
}

Node* MSTNode::deepCopy(bool preserve_id) const {
    MSTNode* copied_node = new MSTNode(node_);
    if (preserve_id) copied_node->id_ = id_;
    copied_node->is_leaf_ = is_leaf_;
    return copied_node;
}

std::vector<MSTNode*> MSTNode::getMSTChildrenNodes() const {
    return MST_children_nodes_;
}

std::vector<Edge*> MSTNode::getMSTChildrenEdges() const {
    return MST_children_edges_;
}

std::unordered_map<MSTNode*, Edge*> MSTNode::getMSTChildrenEdgesMap() const {
    return MST_children_edges_map_;
}

std::string MSTNode::getStringContent() const {
    std::string s = Node::getStringContent() + ",is_leaf=" + std::to_string(is_leaf_);

    s += ";mst_adj_nodes[";
    for(MSTNode* adj_node : getMapKeys<MSTNode*, Edge*>(mst_edges_)) {
        s += std::to_string(adj_node->getId()) + ",";
    }
    s += "]";

    s += ";node_root_info[";
    if (parent_in_rooted_MST_ != nullptr) {
        s += "par(" + std::to_string(parent_in_rooted_MST_->getId()) + "),";
    }
    s += "children(";
    for(MSTNode* child : MST_children_nodes_) {
        s += std::to_string(child->getId()) + ",";
    }
    s += ")";

    s += "]";
    return s;
}

void MSTNode::setRequiredThetaC(double required_theta_c) {
    required_theta_c_ = required_theta_c;
}

MSTNode::~MSTNode() {
    //std::cout << getNodeTypeStr() << "'s Destructor\n";
    delete node_;
}

std::ostream& operator<<(std::ostream& os, const MSTNode& node) {
    os << node.toString();
    return os;
}