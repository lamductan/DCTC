#ifndef DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_
#define DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

#include "dctc/network_components/nodes/node_factory.h"
#include "dctc/network_components/nodes/MST_node.h"
#include "dctc/network_components/edges/edge.h"


class MSTGraph {
protected:
    int n_nodes_ = 0;
    GraphNodeType graph_node_type_ = MST_GRAPH_NODE;
    std::vector<int> node_ids_;
    std::unordered_map<int, Node*> map_id_nodes_;

    long double mst_weight_;
    std::vector<Node*> nodes_;
    std::vector<Edge*> MST_edges_;
    std::vector<std::pair<int, int>> MST_edges_by_id_;
    int max_level_ = 0;
    int n_total_nodes_omni_ = 0;

    std::vector<Edge*> communication_edges_;

    void init(const std::vector<Node*>& nodes, GraphNodeType graph_node_type=MST_GRAPH_NODE);
    void dfs1(MSTNode* u, MSTNode* par, int level);

public:
    MSTGraph();
    MSTGraph(const std::vector<Node*>& nodes, GraphNodeType graph_node_type=MST_GRAPH_NODE, bool build_MST=true);
    GraphNodeType getGraphNodeType() const;
    virtual std::string getGraphTypeStr() const;
    int getNNodes() const;
    std::unordered_map<int, Node*> getMapIdNodes() const;
    std::vector<Node*> getNodes() const;

    static int calculateNumRelaysOmni(Edge* edge, long double r_c);

    long double buildMST();
    long double buildMST(std::vector<Edge*>& graph_edges);
    std::vector<Edge*> getMSTEdges() const;
    long double getMinimumMSTEdgeLength() const;
    long double getMaximumMSTEdgeLength() const;
    long double getMinimumCommunicationEdgeLength() const;
    long double getMaximumCommunicationEdgeLength() const;

    int getNTotalNodesOmni() const;
    void setNTotalNodesOmni(int n_total_nodes_omni);

    MSTNode* rootTree(MSTNode* root);

    Edge* addCommunicationEdge(MSTNode* a, MSTNode* b);
    Edge* orientAndConnect(MSTNode* node_to_orient, MSTNode* node_to_connect);
    std::vector<Edge*> getCommunicationEdges() const;

    virtual MSTGraph* cloneFrom(const MSTGraph* mst_graph, GraphNodeType graph_node_type=MST_GRAPH_NODE);
    virtual MSTGraph* deepCopy(GraphNodeType graph_node_type=MST_GRAPH_NODE) const;
    void save(const std::string& save_path);
    virtual ~MSTGraph();

    friend std::ostream& operator<<(std::ostream& os, const MSTGraph& MST_graph);
};

#endif //DCTC_NETWORK_COMPONENTS_GRAPHS_GRAPH_H_