#include "dctc/algorithms/coverage/coverage_alg.h"
#include "dctc/network_components/nodes/DD_node.h"


std::vector<Node*> trivialCoverageAlg(const Instance* instance) {
    NodeType node_type = instance->getNodeType();
    long double r_s = instance->getRS();
    long double r_c = instance->getRC();
    long double theta_s = instance->getThetaS();
    long double theta_c = instance->getThetaC();
    const std::vector<Point2D>& targets = instance->getTargets();

    std::vector<Node*> nodes;
    for(const Point2D& target : targets) {
        Node* node = new DDNode(target.getX(), target.getY(), node_type, r_s, r_c, theta_s, theta_c);
        nodes.push_back(node);
    }
    return nodes;
}