#include "data_structures/counter.h"
#include "dctc/network_components/edges/edge.h"


int Edge::n_instances = 0;

void Edge::setSegment2D() {
    if (endpoint_1_ != nullptr && endpoint_2_ != nullptr)
        segment2D_ = Segment2D(endpoint_1_->getPoint2D(), endpoint_2_->getPoint2D());
}

void Edge::init(Node* endpoint_1, Node* endpoint_2) {
    endpoint_1_ = endpoint_1;
    endpoint_2_ = endpoint_2;
    endpoint_id_1_ = endpoint_1_->getId();
    endpoint_id_2_ = endpoint_2_->getId();
    setSegment2D();
}

void Edge::init(int endpoint_id_1, int endpoint_id_2) {
    endpoint_id_1_ = endpoint_id_1;
    endpoint_id_2_ = endpoint_id_2;
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

Edge::Edge(int endpoint_id_1, int endpoint_id_2) {
    init(endpoint_id_1, endpoint_id_2);
    id_ = Counter::increase();
}

int Edge::getId() const {return id_;}

int Edge::getEndpointId1() const {return endpoint_id_1_;}

int Edge::getEndpointId2() const {return endpoint_id_2_;}

Node* Edge::getEndpoint1() const {return endpoint_1_;}

Node* Edge::getEndpoint2() const {return endpoint_2_;}

void Edge::setEndpoint1(Node* endpoint1) {
    endpoint_1_ = endpoint1;
    setSegment2D();
}

void Edge::setEndpoint2(Node* endpoint2) {
    endpoint_2_ = endpoint2;
    setSegment2D();
}

int Edge::getOtherEndpointId(int endpoint_id) const {
    return endpoint_id_1_ + endpoint_id_2_ - endpoint_id;
}

Node* Edge::getOtherEndpoint(Node* endpoint) const {
    return (endpoint_1_ == endpoint) ? endpoint_2_ : endpoint_1_;
}

double Edge::length() const {return segment2D_.length();}

std::string Edge::getEdgeTypeStr() const {return "Edge";}

std::string Edge::getStringContent() const {
    return std::to_string(id_) + "(" + std::to_string(endpoint_1_->getId()) + "," + std::to_string(endpoint_2_->getId()) + ")";
}

std::string Edge::toString() const {
    return getEdgeTypeStr() + "[" + getStringContent() + "]";
}

Edge::~Edge() {
    //std::cout << getEdgeTypeStr() << "'s Destructor\n";
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.toString();
    return os;
}