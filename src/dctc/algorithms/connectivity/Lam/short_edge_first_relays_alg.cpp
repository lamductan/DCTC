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
    // Step 1: Connect short subtrees
    std::cout << "Step 1: Connect short subtrees\n";
    for(Node* terminal_ : terminals_) {
        MSTNodeLam* terminal = (MSTNodeLam*) terminal_;
        if (!terminal->free_) continue;
        steinerizeAndOrientShortSubtree(terminal);
    }

    // Step 2: Connect relay nodes on long edges
    std::cout << "Step 2: Connect relay nodes on long edges\n";
    for(Edge* long_or_medium_edge : long_or_medium_edges_) {
        SteinerizeLongOrMediumEdgeResult_LEF steinerize_long_or_medium_edge_result = steinerizeLongOrMediumEdge(
            long_or_medium_edge, graph_node_type_);
    }

    // Step 3: Connect terminals to type-1 relay nodes
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
        orientNodeToBisectorCoverNode(terminal, adj_node);
        setNodeFixed(terminal);
        std::cout << "Subtree " << *(Node*) terminal << '\n';
        dfsShortSubtree(adj_node, terminal, edge);
        std::cout << "============================End subtree=============================\n\n";
    }
}

void ShortEdgeFirstRelaysAlg::dfsShortSubtree(MSTNodeLam* terminal, MSTNodeLam* parent, Edge* edge_to_parent) {
    std::cout << *(Node*) terminal << '\n';
    SteinerizeShortEdgeResult steinerize_short_edge_result = steinerizeShortEdge(edge_to_parent, graph_node_type_);
    relays_ += steinerize_short_edge_result.type_4_relays;
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

    std::vector<Point2D> type_4_relays_pos;
    assert(terminal2->free_ && !terminal1->free_);
    type_4_relays_pos = calculateShortEdgeRelaysPos_OneFree(
        (Sector*) terminal1->getCommunicationAntenna(), (Sector*) terminal2->getCommunicationAntenna(), &Ax);

    if (type_4_relays_pos.empty()) {
        orientNodeToBisectorCoverNode(terminal2, terminal1);
        setNodeFixed(terminal2);
        result.communication_edges.push_back(addCommunicationEdge(terminal1, terminal2));
        return result;
    }
    return steinerizeShortEdgeWithOrientation(terminal1, terminal2, type_4_relays_pos, graph_node_type);
}

Point2D ShortEdgeFirstRelaysAlg::findType3RelayPos(Node* terminal, Node* type_1_relay) {
    std::cout << '\n' << __PRETTY_FUNCTION__ << '\n';
    std::cout << *terminal << ' ' << *type_1_relay << '\n';
    Point2D A = terminal->getPoint2D();
    Sector* sA = (Sector*) terminal->getCommunicationAntenna();
    Point2D B = type_1_relay->getPoint2D();
    Sector* sB = (Sector*) type_1_relay->getCommunicationAntenna();

    Point2D type_3_relay_pos;
    Point2D A1 = sA->getEndpoint1();
    if (sB->containsPoint2D(A1))
        type_3_relay_pos = A1;
    else type_3_relay_pos = sA->getEndpoint2();
    assert(computeGeometricAngle(A, type_3_relay_pos, B) <= PI_2 + EPSILON*2.5);
    return type_3_relay_pos;
}

void ShortEdgeFirstRelaysAlg::connectTerminalWithType1Relays(MSTNodeLam* terminal) {
    std::cout << '\n' << __PRETTY_FUNCTION__ << '\n';
    std::cout << *terminal << '\n';
    std::vector<std::vector<Node*>> type_1_relays_connected_components 
        = findConnectedComponentsType1RelaysSurroundingTerminals(terminal->type_1_relays_);
    for(const std::vector<Node*>& component : type_1_relays_connected_components) {
        std::vector<Edge*> type_1_relays_edges = connectType1RelaysInTheSameComponent(component);
        communication_edges_ += type_1_relays_edges;
    }

    int n_components = type_1_relays_connected_components.size();
    std::vector<bool> can_terminal_connect_components(n_components, false);

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

    for(int i = 0; i < n_components; ++i) {
        const std::vector<Node*>& component = type_1_relays_connected_components[i];
        for(Node* node : component) {
            if (canTwoNodesConnectEachOther(terminal, node)) {
                can_terminal_connect_components[i] = true;
                communication_edges_.push_back(addCommunicationEdge(terminal, node));
            }
        }
    }

    bool can_terminal_connect_all_components = true;
    int cannot_connect_component_id = -1;
    for(int i = 0; i < n_components; ++i)
        if (!can_terminal_connect_components[i]) {
            can_terminal_connect_all_components = false;
            cannot_connect_component_id = i;
            break;
        }

    if (can_terminal_connect_all_components) return;

    Node* type_1_relay = type_1_relays_connected_components[cannot_connect_component_id][0];
    Point2D type_3_relay_pos = findType3RelayPos(terminal, type_1_relay);
    Node* type_3_relay = createRelayNode(type_3_relay_pos, RELAY_DD_NODE_TYPE_3, graph_node_type_, r_c_, theta_c_);
    orientNodeToCoverNodes(type_3_relay, {terminal, type_1_relay});
    setNodeFixed(type_3_relay);
    relays_.push_back(type_3_relay);
    communication_edges_.push_back(addCommunicationEdge(terminal, type_3_relay));
    communication_edges_.push_back(addCommunicationEdge(type_1_relay, type_3_relay));

    if (type_1_relays_connected_components.size() == 2) {
        std::vector<Node*> leaders{type_1_relays_connected_components[0][0], type_1_relays_connected_components[1][0]};
        Point2D B = leaders[0]->getPoint2D();
        Sector* sB = (Sector*) leaders[0]->getCommunicationAntenna();
        Point2D C = leaders[1]->getPoint2D();
        Sector* sC = (Sector*) leaders[1]->getCommunicationAntenna();
        Circle cA(terminal->getPoint2D(), r_c_);
        Line2D perpendicular_bisector_BC = Segment2D(B, C).getPerpendicularBisector();
        std::vector<Point2D> intersections = cA.findIntersectionsWithLine2D(perpendicular_bisector_BC);
        for(Point2D X : intersections) {
            if (sB->containsPoint2D(X) && sC->containsPoint2D(X)) {
                assert(computeGeometricAngle(B, X, C) <= PI_2 + EPSILON*2.5);
                Node* type_3_relay = createRelayNode(type_3_relay_pos, RELAY_DD_NODE_TYPE_3, graph_node_type_, r_c_, theta_c_);
                orientNodeToCoverNodes(type_3_relay, {terminal, type_1_relay});
                setNodeFixed(type_3_relay);
                relays_.push_back(type_3_relay);
                communication_edges_.push_back(addCommunicationEdge(leaders[0], type_3_relay));
                communication_edges_.push_back(addCommunicationEdge(leaders[1], type_3_relay));
                return;
            }
        }
    }
    return;
}