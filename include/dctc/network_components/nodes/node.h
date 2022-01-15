#ifndef DCTC_NETWORK_COMPONENTS_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODE_H_

#include <string>
#include "geometric_primitives/geom2D.h"

enum NodeType {
    SENSING_OMNIDIRECTIONAL_NODE,
    RELAY_OMNIDIRECTIONAL_NODE,
    SENSING_DIRECTIONAL_NODE,
    RELAY_DIRECTIONAL_NODE_TYPE_1,
    RELAY_DIRECTIONAL_NODE_TYPE_2,
    RELAY_DIRECTIONAL_NODE_TYPE_3,
    RELAY_DIRECTIONAL_NODE_TYPE_4,
    RELAY_DIRECTIONAL_NODE_TYPE_5,
};

class Node {
protected:
    static int n_instances;
    int id_;
    double x_;
    double y_;
    Point2D point2D_;
    NodeType node_type_;

public:
    Node();
    Node(double x, double y, NodeType node_type);
    int getId();
    double getX();
    double getY();
    Point2D getPoint2D();
    NodeType getNodeType();
    std::string toString(); 
};

int Node::n_instances = 0;

#endif //DCTC_NETWORK_COMPONENTS_NODE_H_