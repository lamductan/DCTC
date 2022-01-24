#include <algorithm>
#include <vector>
#include <unordered_map>

#include "geometric_primitives/geom2D.h"
#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/communication_checker.h"


bool CommunicationChecker::checkConnectivity(const MSTGraph* MST_graph) {
    std::vector<Node*> nodes = MST_graph->getNodes(); 
    std::unordered_map<Node*, bool> vis;
    dfs((MSTNode*) nodes[0], nullptr, vis);
    for(Node* node : nodes) if (!vis[node]) {
        //std::cout << "Can't reach node " << *(MSTNode*) node << '\n';
        return false;
    }
    return true;
}

void CommunicationChecker::dfs(MSTNode* node, Node* par, std::unordered_map<Node*, bool>& vis) {
    if (node == nullptr) return;
    vis[node] = true;
    for(MSTNode* adj_node : node->getMSTEdgeAdjNodes()) {
        if (adj_node == par || vis[adj_node]) continue;
        dfs(adj_node, node, vis);
    }
}

bool CommunicationChecker::checkConnectivity1(const MSTGraph* MST_graph) {
    std::vector<Node*> nodes = MST_graph->getNodes(); 
    std::unordered_map<Node*, bool> vis;
    dfs1((MSTNode*) nodes[0], nullptr, vis);
    for(Node* node : nodes) if (!vis[node]) {
        //std::cout << "Can't reach node " << *(MSTNode*) node << '\n';
        return false;
    }
    return true;
}

void CommunicationChecker::dfs1(MSTNode* node, Node* par, std::unordered_map<Node*, bool>& vis) {
    if (node == nullptr) return;
    vis[node] = true;
    for(Node* adj_node : node->getCommunicationEdgeAdjNodes()) {
        if (adj_node == par || vis[adj_node]) continue;
        dfs1((MSTNode*) adj_node, node, vis);
    }
}

bool CommunicationChecker::checkSymmetricConnectivity(const MSTGraph* MST_graph) {
    for(Node* node : MST_graph->getNodes())
        if (!checkSymmetricConnectivityOneNode((MSTNode*) node)) return false;
    return true;
}

bool CommunicationChecker::checkSymmetricConnectivityOneNode(MSTNode* node) {
    for(Node* adj_node : node->getMSTEdgeAdjNodes()) {
        if (!node->canCoverOtherNodeByCommunicationAntenna(adj_node)) return false;
        if (!adj_node->canCoverOtherNodeByCommunicationAntenna(node)) return false;
    }
    return true;
}

bool CommunicationChecker::checkSymmetricConnectivityOneNode1(MSTNode* node) {
    for(Node* adj_node : node->getCommunicationEdgeAdjNodes()) {
        if (!node->canCoverOtherNodeByCommunicationAntenna(adj_node)) return false;
        if (!adj_node->canCoverOtherNodeByCommunicationAntenna(node)) return false;
    }
    return true;
}

bool CommunicationChecker::checkAngle(const MSTGraph* MST_graph) {
    for(Node* node : MST_graph->getNodes())
        if (!checkAngleOneNode((MSTNode*) node)) return false;
    return true;
}

bool CommunicationChecker::checkAngleOneNode(MSTNode* node) {
    long double theta_c = node->getThetaC();
    Point2D node_position = node->getPoint2D();
    Point2D original_bisector_unit_point = node_position + Point2D(1, 0);

    std::vector<Point2D> adj_node_points;
    for(Node* adj_node : node->getMSTEdgeAdjNodes()) {
        adj_node_points.push_back(adj_node->getPoint2D());
    }

    std::vector<long double> angles;
    int n = adj_node_points.size();
    for(const Point2D& point : adj_node_points) {
        long double angle_with_original_unit_vector = computeAngle(original_bisector_unit_point, node_position, point);
        angles.push_back(angle_with_original_unit_vector);
    }
    sort(angles.begin(), angles.end());
    for(int i = 0; i < n; ++i) {
        angles.push_back(angles[i] + TWO_PI);
    }

    long double required_theta_c = TWO_PI;
    for(int i = 0; i < n; ++i) {
        required_theta_c = std::min(required_theta_c, angles[i + n - 1] - angles[i]);
    }
    node->setRequiredThetaC(required_theta_c);

    if (required_theta_c > theta_c + EPSILON) {
        std::cout << "required_theta_c - PI/2 = " << required_theta_c - PI/2 << '\n';
    }

    return required_theta_c <= theta_c + EPSILON;
}

bool CommunicationChecker::checkAngleOneNode1(MSTNode* node) {
    long double theta_c = node->getThetaC();
    Point2D node_position = node->getPoint2D();
    Point2D original_bisector_unit_point = node_position + Point2D(1, 0);

    std::vector<Point2D> adj_node_points;
    for(Node* adj_node : node->getCommunicationEdgeAdjNodes()) {
        adj_node_points.push_back(adj_node->getPoint2D());
    }

    std::vector<long double> angles;
    int n = adj_node_points.size();
    for(const Point2D& point : adj_node_points) {
        long double angle_with_original_unit_vector = computeAngle(original_bisector_unit_point, node_position, point);
        angles.push_back(angle_with_original_unit_vector);
    }
    sort(angles.begin(), angles.end());
    for(int i = 0; i < n; ++i) {
        angles.push_back(angles[i] + TWO_PI);
    }

    long double required_theta_c = TWO_PI;
    for(int i = 0; i < n; ++i) {
        required_theta_c = std::min(required_theta_c, angles[i + n - 1] - angles[i]);
    }
    node->setRequiredThetaC(required_theta_c);

    if (required_theta_c > theta_c + EPSILON) {
        std::cout << "required_theta_c - PI/2 = " << required_theta_c - PI/2 << '\n';
    }

    return required_theta_c <= theta_c + EPSILON;
}

bool CommunicationChecker::checkRange(const MSTGraph* MST_graph) {
    for(Node* node : MST_graph->getNodes())
        if (!checkRangeOneNode((MSTNode*) node)) return false;
    return true;
}

bool CommunicationChecker::checkRangeOneNode(MSTNode* node) {
    long double r_c = node->getRC();
    for(Edge* edge : node->getMSTEdges()) {
        if (edge->length() - EPSILON > r_c) return false;
    }
    return true;
}

bool CommunicationChecker::checkConnectivityAndAngle(const MSTGraph* MST_graph) {
    return checkConnectivity(MST_graph) && checkSymmetricConnectivity(MST_graph) && checkAngle(MST_graph);
}

bool CommunicationChecker::checkConnectivityAngleAndRange(const MSTGraph* MST_graph) {
    return checkConnectivity(MST_graph) && checkSymmetricConnectivity(MST_graph)
           && checkAngle(MST_graph) && checkRange(MST_graph);
}