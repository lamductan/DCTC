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
    int endpoint_id_1_;
    int endpoint_id_2_;
    Node* endpoint_1_ = nullptr;
    Node* endpoint_2_ = nullptr;
    void init(Node* endpoint_1, Node* endpoint_2);
    void init(int endpoint_id_1, int endpoint_id_2);
    void setSegment2D();
    virtual std::string getStringContent() const;

public:
    Edge();
    Edge(Node* endpoint_1, Node* endpoint_2);
    Edge(int endpoint_id_1, int endpoint_id_2);
    int getId() const;
    int getEndpointId1() const;
    int getEndpointId2() const;
    Node* getEndpoint1() const;
    Node* getEndpoint2() const;
    void setEndpoint1(Node* endpoint1);
    void setEndpoint2(Node* endpoint2);
    int getOtherEndpointId(int endpoint_id) const;
    Node* getOtherEndpoint(Node* endpoint) const;
    double length() const;
    virtual std::string getEdgeTypeStr() const;
    std::string toString() const;
    virtual ~Edge();

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
};

#endif //DCTC_NETWORK_COMPONENTS_EDGES_EDGE_H_