#include <cassert>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/relays/simple_relays_alg.h"

SimpleRelaysAlg::SimpleRelaysAlg() {}

void SimpleRelaysAlg::init(long double r_c, long double theta_c) {
    MST_edges_ = MST_graph_->getMSTEdges();
    r_c_ = r_c;
    theta_c_ = theta_c;

    terminals_ = MST_graph_->getNodes();
    n_total_nodes_omni_ = MST_graph_->getNTotalNodesOmni();
    std::cout << MST_graph_->getMaximumMSTEdgeLength() << ' ' << r_c_ << ' ' << n_total_nodes_omni_ << '\n';
}

SimpleRelaysAlg::SimpleRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c) {
    MST_graph_ = MST_graph;
    init(r_c, theta_c);
}

bool SimpleRelaysAlg::isShortEdge(Edge* edge) const {return !isLongEdge(edge);}

bool SimpleRelaysAlg::isLongOrMediumEdge(Edge* edge) const {return edge->length() > r_c_;}

bool SimpleRelaysAlg::isLongEdge(Edge* edge) const {return edge->length() > r_c_*2;}

bool SimpleRelaysAlg::isMediumEdge(Edge* edge) const {
    long double length = edge->length();
    return length > r_c_ && length <= r_c_*2;
}

RelaysMSTGraph* SimpleRelaysAlg::solve() {
    int n_long_or_medium_edges = 0;
    for(Edge* MST_edge : MST_edges_) {
        if (isLongOrMediumEdge(MST_edge)) {
            ++n_long_or_medium_edges;
            SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result
                = steinerizeLongOrMediumEdge(MST_edge, graph_node_type_);
            connectTerminalsWithType1Relays(
                MST_edge, steinerize_long_or_medium_edge_result.type_1_relays, communication_edges_);
        } else {
            Edge* communication_edge = addCommunicationEdge(MST_edge->getEndpoint1(), MST_edge->getEndpoint2());
            communication_edges_.push_back(communication_edge);
        }
    }

    int n_type1_relays = 0;
    int n_type2_relays = 0;
    for(Node* relay : relays_) {
        if (relay->getNodeType() == RELAY_DD_NODE_TYPE_1) ++n_type1_relays;
        else if (relay->getNodeType() == RELAY_DD_NODE_TYPE_2) ++n_type2_relays;
    }
    assert(n_type1_relays + n_type2_relays == relays_.size());
    int n = terminals_.size();
    std::cout << "Check size: " << n << '\n';
    std::cout << "Type 1: " << n_type1_relays << ' ' << 2*n_long_or_medium_edges << '\n';
    std::cout << "Type 2: " << n_type2_relays << ' ' << 3*(n_total_nodes_omni_ - terminals_.size()) << '\n';
    nodes_ = terminals_ + relays_;
    return new RelaysMSTGraph(nodes_, graph_node_type_, r_c_, theta_c_, communication_edges_, n_total_nodes_omni_);
}

void SimpleRelaysAlg::connectTerminalsWithType1Relays(
    Edge* edge, const std::vector<Node*>& type_1_relays, std::vector<Edge*>& communication_edges
) const {
    assert(type_1_relays[0]->canCoverOtherNodeByCommunicationAntenna(edge->getEndpoint1()));
    communication_edges.push_back(addCommunicationEdge(type_1_relays[0], edge->getEndpoint1()));
    assert(type_1_relays[1]->canCoverOtherNodeByCommunicationAntenna(edge->getEndpoint2()));
    communication_edges.push_back(addCommunicationEdge(type_1_relays[1], edge->getEndpoint2()));
}

void SimpleRelaysAlg::connectType1Type2Relays(
    const std::vector<Node*>& type_1_relays, const std::vector<Node*>& type_2_relays,
    std::vector<Edge*>& communication_edges
) const {
    //Connect type-1 relays to their corresponding type-2 relays
    Node* type_2_relay_0 = type_2_relays[0];
    orientNodeToBisectorCoverNode(type_2_relay_0, type_1_relays[0]);
    assert(type_2_relay_0->canCoverOtherNodeByCommunicationAntenna(type_1_relays[0]));
    communication_edges.push_back(addCommunicationEdge(type_2_relay_0, type_1_relays[0])); 
    Node* type_2_relay_last = type_2_relays.back();
    orientNodeToBisectorCoverNode(type_2_relay_last, type_1_relays[1]);
    assert(type_2_relay_last->canCoverOtherNodeByCommunicationAntenna(type_1_relays[1]));
    communication_edges.push_back(addCommunicationEdge(type_2_relay_last, type_1_relays[1])); 

    //Orient and Connect type-2 relays
    for(int i = 1; i < type_2_relays.size() - 1; ++i) {
        Node* node_to_orient = type_2_relays[i];
        std::vector<Node*> nodes_to_cover = {type_2_relays[i - 1], type_2_relays[i + 1]};
        orientNodeToCoverNodes(node_to_orient, nodes_to_cover);
    }
    for(int i = 0; i < type_2_relays.size() - 1; ++i) {
        communication_edges.push_back(addCommunicationEdge(type_2_relays[i], type_2_relays[i + 1]));
    }
}

SteinerizeLongOrMediumEdgeResult_LEF SimpleRelaysAlg::steinerizeLongOrMediumEdgesWithOrientation(
    Edge* edge, const std::pair<Point2D, Point2D>& type_1_relays_pos, const std::vector<Point2D>& type_2_relays_pos,
    GraphNodeType graph_node_type
) const {
    Node* type_1_relay_0 = createRelayNode(
        type_1_relays_pos.first, RELAY_DD_NODE_TYPE_1, graph_node_type, r_c_, theta_c_);
    Node* type_1_relay_1 = createRelayNode(
        type_1_relays_pos.second, RELAY_DD_NODE_TYPE_1, graph_node_type, r_c_, theta_c_);
    std::vector<Node*> type_1_relays{type_1_relay_0, type_1_relay_1};

    std::vector<Node*> type_2_relays;
    for(const Point2D& type_2_relay_pos : type_2_relays_pos) {
        Node* type_2_relay = createRelayNode(type_2_relay_pos, RELAY_DD_NODE_TYPE_2, graph_node_type, r_c_, theta_c_);
        type_2_relays.push_back(type_2_relay);
    }

    std::vector<Edge*> communication_edges;
    connectType1Type2Relays(type_1_relays, type_2_relays, communication_edges);

    //Return result
    SteinerizeLongOrMediumEdgeResult_LEF result;
    result.type_1_relays = type_1_relays;
    result.type_2_relays = type_2_relays;
    result.communication_edges = communication_edges;
    return result;
}

SteinerizeLongOrMediumEdgeResult_LEF SimpleRelaysAlg::steinerizeLongEdges(
    Edge* long_edge, GraphNodeType graph_node_type
) const {
    Segment2D segment2D = long_edge->getSegment2D();
    std::pair<Point2D, Point2D> type_1_relays_pos = calculateRelaysType1Positions(segment2D, r_c_); 
    std::vector<Point2D> type_2_relays_pos = calculateRelaysType2Positions_LongEdge(
        segment2D, type_1_relays_pos, r_c_);
    return steinerizeLongOrMediumEdgesWithOrientation(
        long_edge, type_1_relays_pos, type_2_relays_pos, graph_node_type);
}

SteinerizeLongOrMediumEdgeResult_LEF SimpleRelaysAlg::steinerizeMediumEdge(
    Edge* medium_edge, GraphNodeType graph_node_type
) const {
    Segment2D segment2D = medium_edge->getSegment2D();
    std::pair<Point2D, Point2D> type_1_relays_pos = calculateRelaysType1Positions(segment2D, r_c_); 
    std::vector<Point2D> type_2_relays_pos = calculateShortEdgeRelaysPos_TwoNonFree(
        Segment2D(type_1_relays_pos.first, type_1_relays_pos.second));
    return steinerizeLongOrMediumEdgesWithOrientation(
        medium_edge, type_1_relays_pos, type_2_relays_pos, graph_node_type);
}

SteinerizeLongOrMediumEdgeResult_LEF SimpleRelaysAlg::steinerizeLongOrMediumEdge(
    Edge* long_or_medium_edge, GraphNodeType graph_node_type
) {
    SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result;
     if (isMediumEdge(long_or_medium_edge)) 
        steinerize_long_or_medium_edge_result = steinerizeMediumEdge(long_or_medium_edge, graph_node_type);
    else steinerize_long_or_medium_edge_result = steinerizeLongEdges(long_or_medium_edge, graph_node_type);
    orientNodeToBisectorCoverNode(
        steinerize_long_or_medium_edge_result.type_1_relays[0], long_or_medium_edge->getEndpoint1());
    orientNodeToBisectorCoverNode(
        steinerize_long_or_medium_edge_result.type_1_relays[1], long_or_medium_edge->getEndpoint2());
    relays_ += steinerize_long_or_medium_edge_result.type_1_relays;
    relays_ += steinerize_long_or_medium_edge_result.type_2_relays;
    communication_edges_ += steinerize_long_or_medium_edge_result.communication_edges;
    return steinerize_long_or_medium_edge_result;
}

GraphNodeType SimpleRelaysAlg::getGraphNodeType() const {return graph_node_type_;}

long double SimpleRelaysAlg::getThetaC() const {return theta_c_;}

long double SimpleRelaysAlg::getRC() const {return r_c_;}

int SimpleRelaysAlg::getNumTerminals() const {return terminals_.size();}

int SimpleRelaysAlg::getNumRelays() const {return relays_.size();}

int SimpleRelaysAlg::getNumTotalNodes() const {return nodes_.size();}

int SimpleRelaysAlg::getNumTotalNodesOmni() const {return n_total_nodes_omni_;}

std::vector<Node*> SimpleRelaysAlg::getTerminals() const {return terminals_;}

std::vector<Node*> SimpleRelaysAlg::getRelayNodes() const {return relays_;}

std::vector<Node*> SimpleRelaysAlg::getAllNodes() const {return nodes_;}

std::vector<Edge*> SimpleRelaysAlg::getCommunicationEdges() const {return communication_edges_;}

SimpleRelaysAlg::~SimpleRelaysAlg() {
    for(Edge* edge : communication_edges_) delete edge;
    for(Node* node : relays_) delete node;
}