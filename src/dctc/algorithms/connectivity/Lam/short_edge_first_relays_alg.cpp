#include <cassert>

#include "dctc/algorithms/connectivity/Lam/short_edge_first_relays_alg.h"
#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/algorithms/connectivity/communication_checker.h"
#include "dctc/algorithms/connectivity/relays/relays_utils.h"
#include "dctc/algorithms/connectivity/Lam/long_edge_first_relays_alg.h"


ShortEdgeFirstRelaysAlg::ShortEdgeFirstRelaysAlg(MSTGraph* MST_graph, long double r_c, long double theta_c)
: LongShortEdgeRelaysAlg(MST_graph, r_c, theta_c) {}

RelaysMSTGraph* ShortEdgeFirstRelaysAlg::solve() {
    // Step 1: Connect short subtrees by type-3 relays
    std::cout << "Step 1: Connect short subtrees\n";
    for(Node* terminal_ : terminals_) {
        MSTNodeLam* terminal = (MSTNodeLam*) terminal_;
        if (!terminal->free_) continue;
        steinerizeAndOrientShortSubtree(terminal);
    }

    // Step 2: Connect type-1 relays on long edges by type-2 relays
    std::cout << "Step 2: Connect relay nodes on long edges\n";
    for(Edge* long_or_medium_edge : long_or_medium_edges_) {
        SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result = steinerizeLongOrMediumEdge(
            long_or_medium_edge, graph_node_type_);
    }

    // Step 3: Connect terminals to their corresponding type-1 relays by type-4 relays
    std::cout << "Step 3: Connect terminals to type-1 relay nodes\n";
    for(Node* terminal_ : terminals_) {
        MSTNodeLam* terminal = (MSTNodeLam*) terminal_;
        if (terminal->type_1_relays_.empty()) continue;
        connectTerminalWithType1Relays(terminal);
    }

    return statAndReturnRelaysMSTGraph();
}

void ShortEdgeFirstRelaysAlg::steinerizeAndOrientShortSubtree(MSTNodeLam* terminal) {
    for(auto it : terminal->getMSTEdgesMap()) {
        MSTNodeLam* adj_node = (MSTNodeLam*) it.first;
        Edge* edge = it.second;
        if (!isShortEdge(edge)) continue;
        if (terminal->free_) {
            orientNodeToBisectorCoverNode(terminal, adj_node);
            setNodeFixed(terminal);
        }
        dfsShortSubtree(adj_node, terminal, edge);
    }
}

void ShortEdgeFirstRelaysAlg::dfsShortSubtree(MSTNodeLam* terminal, MSTNodeLam* parent, Edge* edge_to_parent) {
    SteinerizeShortEdgeResult steinerize_short_edge_result = steinerizeShortEdge(edge_to_parent, graph_node_type_);
    relays_ += steinerize_short_edge_result.type_3_relays;
    communication_edges_ += steinerize_short_edge_result.communication_edges;

    for(auto it : terminal->getMSTEdgesMap()) {
        MSTNodeLam* adj_node = (MSTNodeLam*) it.first;
        if (adj_node == parent) continue;
        Edge* edge = it.second;
        if (!isShortEdge(edge)) continue;
        dfsShortSubtree(adj_node, terminal, edge);
    }
}

SteinerizeShortEdgeResult ShortEdgeFirstRelaysAlg::steinerizeShortEdge(
    Edge* short_edge, GraphNodeType graph_node_type
) const {
    SteinerizeShortEdgeResult result;
    MSTNodeLam* terminal1 = (MSTNodeLam*) short_edge->getEndpoint1();
    MSTNodeLam* terminal2 = (MSTNodeLam*) short_edge->getEndpoint2();

    if (terminal1->free_) std::swap(terminal1, terminal2);

    int n_children = 0;
    int child_id = 0;
    for(auto it : terminal1->getMSTEdgesMap()) {
        MSTNodeLam* adj_node = (MSTNodeLam*) it.first;
        Edge* edge = it.second;
        if (!isShortEdge(edge)) continue;
        if (adj_node == terminal2) child_id = n_children;
        ++n_children;
    }

    Point2D A = terminal1->getPoint2D();
    Sector* sA = (Sector*) terminal1->getCommunicationAntenna();
    Point2D B = sA->getEndpoint1();
    Point2D C = rotate(B, A, sA->getAngle()/n_children*child_id);
    Ray2D Ax = Ray2D::fromTwoPoints(A, C);

    std::vector<Point2D> type_3_relays_pos;
    assert(terminal2->free_ && !terminal1->free_);
    type_3_relays_pos = calculateShortEdgeRelaysPos_OneFree(
        (Sector*) terminal1->getCommunicationAntenna(), (Sector*) terminal2->getCommunicationAntenna(), &Ax);

    if (type_3_relays_pos.empty()) {
        orientNodeToBisectorCoverNode(terminal2, terminal1);
        setNodeFixed(terminal2);
        result.communication_edges.push_back(addCommunicationEdge(terminal1, terminal2));
        return result;
    }
    return steinerizeShortEdgeWithOrientation(terminal1, terminal2, type_3_relays_pos, graph_node_type);
}

Point2D ShortEdgeFirstRelaysAlg::findType4RelayPos(Node* terminal, Node* type_1_relay) {
    Point2D A = terminal->getPoint2D();
    Sector* sA = (Sector*) terminal->getCommunicationAntenna();
    Point2D B = type_1_relay->getPoint2D();
    Sector* sB = (Sector*) type_1_relay->getCommunicationAntenna();

    Point2D type_4_relay_pos;
    Point2D E1 = sA->getEndpoint1();
    Point2D E2 = sA->getEndpoint2();
    Point2D A1 = (A + E1) / 2;
    Point2D A2 = (A + E2) / 2;
    if (sB->containsPoint2D(A1))
        type_4_relay_pos = A1;
    else { 
        type_4_relay_pos = A2;
        if (!sB->containsPoint2D(type_4_relay_pos)) {
            std::cout << "BUG: " << __PRETTY_FUNCTION__ << '\n';
            std::cout << *sA << '\n';
            std::cout << *sB << '\n';
            std::cout << type_4_relay_pos << '\n';
            assert(false);
        }
    }
    assert(computeGeometricAngle(A, type_4_relay_pos, B) <= PI_2 + EPSILON*2.5);
    assert(sA->containsPoint2D(type_4_relay_pos));
    assert(sB->containsPoint2D(type_4_relay_pos));
    return type_4_relay_pos;
}

void ShortEdgeFirstRelaysAlg::connectTerminalWithType1Relays(MSTNodeLam* terminal) {
    std::vector<std::vector<Node*>> type_1_relays_connected_components 
        = findConnectedComponentsType1RelaysSurroundingTerminals(terminal->type_1_relays_);
    for(const std::vector<Node*>& component : type_1_relays_connected_components) {
        std::vector<Edge*> type_1_relays_edges = connectType1RelaysInTheSameComponent(component);
        communication_edges_ += type_1_relays_edges;
    }

    int n_components = type_1_relays_connected_components.size();
    std::vector<bool> can_terminal_connect_components(n_components, false);

    // Terminal is free, can orient it to connect with their type-1 relays
    if (terminal->free_) {
        if (n_components == 1) {
            Node* type_1_relay = type_1_relays_connected_components[0][0];
            orientNodeToBisectorCoverNode(terminal, type_1_relay);
            communication_edges_.push_back(addCommunicationEdge(terminal, type_1_relay));
        } else {
            std::vector<Node*> leaders{type_1_relays_connected_components[0][0], type_1_relays_connected_components[1][0]};
            orientNodeToCoverNodes(terminal, leaders);
            for(Node* leader : leaders) {
                communication_edges_.push_back(addCommunicationEdge(terminal, leader));
            }
        }
        setNodeFixed(terminal);
        return;
    }

    // Terminal is not free, must use at most two type-4 relays
    for(int i = 0; i < n_components; ++i) {
        const std::vector<Node*>& component = type_1_relays_connected_components[i];
        for(Node* node : component) {
            if (canTwoNodesConnectEachOther(terminal, node)) {
                can_terminal_connect_components[i] = true;
                communication_edges_.push_back(addCommunicationEdge(terminal, node));
            }
        }
    }

    std::vector<int> id_components_cannot_connect;
    for(int i = 0; i < n_components; ++i)
        if (!can_terminal_connect_components[i]) {
            id_components_cannot_connect.push_back(i);
        }

    int m = id_components_cannot_connect.size();
    if (m == 0) return;

    int id_component = id_components_cannot_connect[0];
    Node* type_1_relay = type_1_relays_connected_components[id_component][0];
    Point2D type_4_relay_pos_1 = findType4RelayPos(terminal, type_1_relay);
    Node* type_4_relay_1 = createRelayNode(type_4_relay_pos_1, RELAY_DD_NODE_TYPE_4, graph_node_type_, r_c_, theta_c_);
    orientNodeToCoverNodes(type_4_relay_1, {terminal, type_1_relay});
    setNodeFixed(type_4_relay_1);
    relays_.push_back(type_4_relay_1);
    communication_edges_.push_back(addCommunicationEdge(terminal, type_4_relay_1));
    communication_edges_.push_back(addCommunicationEdge(type_1_relay, type_4_relay_1));

    if (m == 2) {
        std::vector<Node*> leaders{type_1_relays_connected_components[0][0], type_1_relays_connected_components[1][0]};
        Point2D B = leaders[0]->getPoint2D();
        Sector* sB = (Sector*) leaders[0]->getCommunicationAntenna();
        Point2D C = leaders[1]->getPoint2D();
        Sector* sC = (Sector*) leaders[1]->getCommunicationAntenna();
        Circle cA(terminal->getPoint2D(), r_c_/2);
        Line2D perpendicular_bisector_BC = Segment2D(B, C).getPerpendicularBisector();
        std::vector<Point2D> intersections = cA.findIntersectionsWithLine2D(perpendicular_bisector_BC);
        for(Point2D X : intersections) {
            if (sB->containsPoint2D(X) && sC->containsPoint2D(X)) {
                assert(computeGeometricAngle(B, X, C) <= PI_2 + EPSILON*2.5);
                Node* type_4_relay_2 = createRelayNode(X, RELAY_DD_NODE_TYPE_4, graph_node_type_, r_c_, theta_c_);
                orientNodeToCoverNodes(type_4_relay_2, leaders);
                setNodeFixed(type_4_relay_2);
                relays_.push_back(type_4_relay_2);
                communication_edges_.push_back(addCommunicationEdge(leaders[0], type_4_relay_2));
                communication_edges_.push_back(addCommunicationEdge(leaders[1], type_4_relay_2));
                return;
            }
        }
        assert(false);
    }
}