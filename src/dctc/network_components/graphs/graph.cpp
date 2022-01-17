#include <algorithm>

#include "data_structures/counter.h"
#include "data_structures/union_find.h"
#include "dctc/network_components/graphs/graph.h"


MSTGraph::MSTGraph() {}

MSTGraph::MSTGraph(const std::vector<Node*>& nodes) {
    nodes_ = nodes;
    n_nodes_ = nodes.size();
    mst_weight_ = buildMST();
}

std::string MSTGraph::getGraphTypeStr() const {return "MSTGraph";}

int MSTGraph::getNNodes() const {return n_nodes_;}

std::vector<Node*> MSTGraph::getNodes() const {return nodes_;}

std::vector<Edge*> MSTGraph::getMSTEdges() const {return MST_edges_;}

std::unordered_map<Node*, std::vector<Edge*>> MSTGraph::getMSTEdgesAdjNodes() const {return MST_edges_adj_nodes_;}

double MSTGraph::buildMST() {
    UnionFind union_find(Counter::get());

    for(int i = 0; i < n_nodes_; ++i) {
        for(int j = i + 1; j < n_nodes_; ++j) {
            Edge* edge = new Edge(nodes_[i], nodes_[j]);
            complete_edges_.push_back(edge);
        }
    }

    double mst_weight = 0;
    int n_mst_edges = 0;
    std::sort(complete_edges_.begin(), complete_edges_.end(),
              [](Edge* a, Edge* b) {return a->length() < b->length();});

    for(Edge* edge : complete_edges_) {
        int fr = edge->getEndpoint1()->getId();
        int to = edge->getEndpoint2()->getId();
        if (union_find.find(fr) == union_find.find(to)) continue;
        union_find.join(fr, to);
        mst_weight += edge->length();
        ++n_mst_edges;
        MST_edges_.push_back(edge);
        MST_edges_adj_nodes_[edge->getEndpoint1()].push_back(edge);
        MST_edges_adj_nodes_[edge->getEndpoint2()].push_back(edge);
        if (n_mst_edges == n_nodes_ - 1) break;
    }
    return mst_weight;
}

MSTGraph::~MSTGraph() {
    std::cout << getGraphTypeStr() << "'s Destructor\n";
    for(Edge* edge : complete_edges_) delete edge;
    for(Node* node : nodes_) delete node;
}

std::ostream& operator<<(std::ostream& os, const MSTGraph& MST_graph) {
    std::string tab = "    ";
    os << "MSTGraph{" << '\n';
    os << tab << "Nodes[" << '\n';
    for(const Node* node : MST_graph.getNodes()) {
        os << tab << tab << *node << '\n';
    }
    os << tab << "]\n";
    os << tab << "MST_edges[" << '\n';
    for(const Edge* edge : MST_graph.getMSTEdges()) {
        os << tab << tab << *edge << '\n';
    }
    os << tab << "]\n";
    os << "}";
    return os;
}