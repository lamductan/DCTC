#include <string>
#include "dctc/network_components/nodes/DD_node.h"
#include "geometric_primitives/geom2D.h"


void DDNode::init(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s, double theta_c) {
    Node::init(x, y, node_type);
    sensing_sector_ = Sector(point2D_, r_s, theta_s);
    communication_antenna_ = Sector(point2D_, r_c, theta_c);
}

DDNode::DDNode() {
    init(0, 0, SENSING_DD_NODE, 1.0, 1.0, PI_2, PI_3);
}

DDNode::DDNode(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s, double theta_c) {
    init(x, y, node_type, r_s, r_c, theta_s, theta_c);
}

DDNode::DDNode(const Point2D& point2D, NodeType node_type, double r_s, double r_c, double theta_s, double theta_c) {
    init(point2D.getX(), point2D.getY(), node_type, r_s, r_c, theta_s, theta_c);
}

DDNode::DDNode(const DDNode& other) {
    Node::init(other.x_, other.y_, other.node_type_);
    sensing_sector_ = other.sensing_sector_;
    communication_antenna_ = other.communication_antenna_;
}

std::string DDNode::getNodeTypeStr() const {return "DDNode";}

std::string DDNode::getStringContent() const {
    std::string content = Node::getStringContent();
    content += "," + sensing_sector_.toString();
    content += "," + communication_antenna_.toString();
    return content;
}

Node* DDNode::deepCopy(bool preserve_id) const {
    Node* new_node_ptr = new DDNode(*this);
    if (preserve_id) new_node_ptr->setId(id_);
    return new_node_ptr;
}

Circle* DDNode::getSensingSector() const {return (Circle*) &sensing_sector_;}

Circle* DDNode::getCommunicationAntenna() const {return (Circle*) &communication_antenna_;}