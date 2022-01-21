#ifndef DCTC_ALGORITHMS_CONNECTIVITY_CHECKER_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_CHECKER_H_

#include <unordered_map>
#include "dctc/network_components/graphs/MST_graph.h"


class CommunicationChecker {
private:
    static void dfs(MSTNode* node, Node* par, std::unordered_map<Node*, bool>& vis);
    static void dfs1(MSTNode* node, Node* par, std::unordered_map<Node*, bool>& vis);
    static bool checkAngleOneNode(MSTNode* node);
    static bool checkRangeOneNode(MSTNode* node);

public:
    static bool checkConnectivity(const MSTGraph* MST_graph);
    static bool checkConnectivity1(const MSTGraph* MST_graph);

    static bool checkAngle(const MSTGraph* MST_graph);
    static bool checkAngleOneNode1(MSTNode* node);

    static bool checkRange(const MSTGraph* MST_graph);

    static bool checkConnectivityAndAngle(const MSTGraph* MST_graph);
    static bool checkConnectivityAngleAndRange(const MSTGraph* MST_graph);
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_CHECKER_H_