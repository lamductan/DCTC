#ifndef DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_
#define DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_

#include <iostream>
#include <string>
#include "dctc/network_components/nodes/node.h"
#include "geometric_primitives/geom2D.h"


class Edge {
protected:
    static int n_instances;
    int id_;
    Segment2D segment2D_;
    Node* endpoint_1_;
    Node* endpoint_2_;
    void init(Node* endpoint_1, Node* endpoint_2);
    virtual std::string getStringContent() const;

public:
    Edge();
    Edge(Node* endpoint_1, Node* endpoint_2);
    int getId() const;
    Node* getEndpoint1() const;
    Node* getEndpoint2() const;
    double length() const;
    virtual std::string getEdgeTypeStr() const;
    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
};

#endif //DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_