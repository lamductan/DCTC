#ifndef DCTC_NETWORK_COMPONENTS_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODE_H_

#include <iostream>
#include <string>
#include <vector>
#include "geometric_primitives/geom2D.h"


enum NodeType {
    BASE_NODE,
    SENSING_OO_NODE,
    SENSING_OD_NODE,
    SENSING_DO_NODE,
    SENSING_DD_NODE,
    RELAY_OO_NODE,
    RELAY_OD_NODE,
    RELAY_DO_NODE,
    RELAY_DD_NODE_TYPE_1,
    RELAY_DD_NODE_TYPE_2,
    RELAY_DD_NODE_TYPE_3,
    RELAY_DD_NODE_TYPE_4,
    RELAY_DD_NODE_TYPE_5,
};

class Node {
friend class Edge;
friend class MSTGraph;

protected:
    int id_;
    double x_;
    double y_;
    Point2D point2D_;
    NodeType node_type_;
    
    void init(double x, double y, NodeType node_type);
    virtual std::string getStringContent() const;

public:
    Node();
    Node(double x, double y, NodeType node_type);
    Node(const Point2D& point2D, NodeType node_type);
    Node(const Node& other);
    int getId() const;
    double getX() const;
    double getY() const;
    Point2D getPoint2D() const;
    NodeType getNodeType() const;
    virtual std::string getNodeTypeStr() const;
    std::string toString() const;
    virtual Node* deepCopy() const;
    virtual Circle* getSensingSector() const;
    virtual Circle* getCommunicationAntenna() const;

    virtual ~Node();

    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

std::vector<Node*> deepCopy(const std::vector<Node*>& nodes);

#endif //DCTC_NETWORK_COMPONENTS_NODE_H_