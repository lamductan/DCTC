#include <string>
#include "dctc/network_components/nodes/DO_node.h"
#include "geometric_primitives/geom2D.h"


void DONode::init(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s) {
    Node::init(x, y, node_type);
    sensing_sector_ = Sector(point2D_, r_s, theta_s);
    communication_antenna_ = Circle(point2D_, r_c);
}

DONode::DONode() {
    init(0, 0, SENSING_DO_NODE, 1.0, 1.0, PI_2);
}

DONode::DONode(const DONode& other) {
    Node::init(other.x_, other.y_, other.node_type_);
    sensing_sector_ = other.sensing_sector_;
    communication_antenna_ = other.communication_antenna_;
}

DONode::DONode(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s) {
    init(x, y, node_type, r_s, r_c, theta_s);
}

DONode::DONode(const Point2D& point2D, NodeType node_type, double r_s, double r_c, double theta_s) {
    init(point2D.getX(), point2D.getY(), node_type, r_s, r_c, theta_s);
}

std::string DONode::getNodeTypeStr() const {return "DONode";}

std::string DONode::getStringContent() const {
    std::string content = Node::getStringContent();
    content += "," + sensing_sector_.toString();
    content += "," + communication_antenna_.toString();
    return content;
}

Node* DONode::deepCopy(bool preserve_id) const {
    Node* new_node_ptr = new DONode(*this);
    if (preserve_id) new_node_ptr->setId(id_);
    return new_node_ptr;
}

Circle* DONode::getSensingSector() const {return (Circle*) &sensing_sector_;}

Circle* DONode::getCommunicationAntenna() const {return (Circle*) &communication_antenna_;}