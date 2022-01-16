#include "data_structures/counter.h"
#include "dctc/network_components/edges/edge.h"


int Edge::n_instances = 0;

void Edge::init(Node* endpoint_1, Node* endpoint_2) {
    endpoint_1_ = endpoint_1;
    endpoint_2_ = endpoint_2;
    segment2D_ = Segment2D(endpoint_1_->getPoint2D(), endpoint_2_->getPoint2D());
}

Edge::Edge() {
    Node endpoint1 = Node(0, 0, SENSING_OO_NODE);
    Node endpoint2 = Node(1, 1, SENSING_OO_NODE);
    init(&endpoint1, &endpoint2);
    id_ = Counter::increase();
}

Edge::Edge(Node* endpoint_1, Node* endpoint_2) {
    init(endpoint_1, endpoint_2);
    id_ = Counter::increase();
}

int Edge::getId() const {return id_;}

Node* Edge::getEndpoint1() const {return endpoint_1_;}

Node* Edge::getEndpoint2() const {return endpoint_2_;}

double Edge::length() const {return segment2D_.length();}

std::string Edge::getEdgeTypeStr() const {return "Edge";}

std::string Edge::getStringContent() const {
    return std::to_string(id_) + ",\n     " + endpoint_1_->toString() + ",\n     " + endpoint_2_->toString();
}

std::string Edge::toString() const {
    return getEdgeTypeStr() + "[" + getStringContent() + "]";
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.toString();
    return os;
}