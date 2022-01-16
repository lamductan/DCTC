#ifndef DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_
#define DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_

#include <iostream>
#include <vector>
#include <unordered_map>

#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"


class MSTGraph {
private:
    int n_nodes_ = 0;
    double mst_weight_;
    std::vector<Node*> nodes_;
    std::vector<Edge*> complete_edges_;
    std::vector<Edge*> MST_edges_;
    std::unordered_map<Node*, std::vector<Edge*>> MST_edges_adj_nodes_;

    void init(std::vector<Node*> nodes);
    double buildMST();

public:
    MSTGraph();
    MSTGraph(const std::vector<Node*>& nodes);
    int getNNodes() const;
    std::vector<Node*> getNodes() const;
    std::vector<Edge*> getMSTEdges() const;
    std::unordered_map<Node*, std::vector<Edge*>> getMSTEdgesAdjNodes() const;
    virtual ~MSTGraph();

    friend std::ostream& operator<<(std::ostream& os, const MSTGraph& MST_graph);
};

#endif //DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_