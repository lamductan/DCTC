#include <iostream>
#include <string>

#include "data_structures/counter.h"
#include "dctc/network_components/nodes/node.h"


void Node::init(double x, double y, NodeType node_type) {
    x_ = x;
    y_ = y;
    node_type_ = node_type;
    point2D_ = Point2D(x, y);
}

Node::Node() {
    init(0, 0, BASE_NODE);
    id_ = Counter::increase();
}

Node::Node(double x, double y, NodeType node_type) {
    init(x, y, node_type);
    id_ = Counter::increase();
}

Node::Node(const Node& other) {
    init(other.x_, other.y_, other.node_type_);
    id_ = Counter::increase();
}

int Node::getId() const {return id_;}

double Node::getX() const {return x_;}

double Node::getY() const {return y_;}

Point2D Node::getPoint2D() const {return point2D_;}

std::string Node::getStringContent() const {
    return point2D_.toString() + "," + std::to_string(id_) + "," + std::to_string(node_type_);
}

std::string Node::getNodeTypeStr() const {return "Node";}

std::string Node::toString() const {
    return getNodeTypeStr() + "(" + getStringContent() + ")"; 
}

Node* Node::deepCopy() const {
    Node* new_node_ptr = new Node(*this);
    return new_node_ptr;
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << node.toString();
    return os;
}

std::vector<Node*> deepCopy(const std::vector<Node*>& nodes) {
    std::vector<Node*> deep_copied_nodes;
    for(Node* node : nodes) deep_copied_nodes.push_back(node->deepCopy());
    return deep_copied_nodes;
}