#ifndef DCTC_ALGORITHMS_CONNECTIVITY_ASCHNER_MST_GRAPH_ASCHNER_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_ASCHNER_MST_GRAPH_ASCHNER_H_

#include <unordered_map>
#include "dctc/network_components/graphs/MST_graph.h"


class MSTGraphAschner : public MSTGraph {
protected:
    std::vector<std::vector<Node*>> groups_;
    std::unordered_map<Node*, bool> nodes_oriented_;

    void dfs_preorder(Node* u, Node* par, std::vector<Node*>& nodes_in_dfs_preorder) const;
    Node* findBestNodeToConnect(Node* node, const std::vector<Node*>& candidates);

public:
    MSTGraphAschner(const MSTGraph* MST_graph);
    void partition();
    void establishCommunicationGraph();
    MSTGraph* doAllSteps();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_ASCHNER_MST_GRAPH_ASCHNER_H_