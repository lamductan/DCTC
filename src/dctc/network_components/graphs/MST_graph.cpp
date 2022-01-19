#include <algorithm>

#include "data_structures/counter.h"
#include "data_structures/union_find.h"
#include "dctc/utils.h"
#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/graphs/MST_graph.h"


MSTGraph::MSTGraph() {}

void MSTGraph::init(const std::vector<Node*>& nodes) {
    for(Node* node : nodes) {
        MSTNode* mst_node = new MSTNode(node);
        nodes_.push_back(mst_node);
        int id = mst_node->getId();
        map_id_nodes_[id] = mst_node;
        node_ids_.push_back(id);
    }
    n_nodes_ = nodes.size();
}

void MSTGraph::initMSTNodes(const std::vector<Node*>& nodes) {
    for(Node* node : nodes) {
        MSTNode* mst_node = new MSTNode((MSTNode*) node);
        nodes_.push_back(mst_node);
        int id = mst_node->getId();
        map_id_nodes_[id] = mst_node;
        node_ids_.push_back(id);
    }
    n_nodes_ = nodes.size();
}

MSTGraph::MSTGraph(const std::vector<Node*>& nodes) {
    init(nodes);
    mst_weight_ = buildMST();
}

std::string MSTGraph::getGraphTypeStr() const {return "MSTGraph";}

int MSTGraph::getNNodes() const {return n_nodes_;}

std::vector<Node*> MSTGraph::getNodes() const {return nodes_;}

std::vector<Edge*> MSTGraph::getMSTEdges() const {return MST_edges_;}

std::vector<Edge*> MSTGraph::getCommunicationEdges() const {return communication_edges_;}

double MSTGraph::buildMST() {
    UnionFind union_find(Counter::get());
    std::vector<Edge*> complete_edges_;

    for(int i : node_ids_) {
        for(int j : node_ids_) {
            if (j <= i) continue;
            Edge* edge = new Edge(nodes_[i], nodes_[j]);
            complete_edges_.push_back(edge);
        }
    }

    double mst_weight = 0;
    int n_mst_edges = 0;
    std::sort(complete_edges_.begin(), complete_edges_.end(),
              [](Edge* a, Edge* b) {return a->length() < b->length();});

    for(Edge* edge : complete_edges_) {
        if (n_mst_edges < n_nodes_ - 1) {
            int fr = edge->getEndpoint1()->getId();
            int to = edge->getEndpoint2()->getId();
            if (union_find.find(fr) == union_find.find(to)) continue;
            union_find.join(fr, to);
            mst_weight += edge->length();
            ++n_mst_edges;
            MST_edges_.push_back(edge);
            MSTNode* endpoint1 = (MSTNode*) edge->getEndpoint1();
            endpoint1->addMSTEdge(edge);
            MSTNode* endpoint2 = (MSTNode*) edge->getEndpoint2();
            endpoint2->addMSTEdge(edge);
            MST_edges_by_id_.push_back({fr, to});
        } else {
            delete edge;
        }
    }

    for(Node* node : nodes_) {
        MSTNode* mst_node = (MSTNode*) node;
        mst_node->setIsLeaf(mst_node->getMSTEdgeAdjNodes().size() == 1);
    } 
    return mst_weight;
}

MSTGraph* MSTGraph::deepCopy() const {
    MSTGraph* copied_mst_graph = new MSTGraph();
    copied_mst_graph->initMSTNodes(nodes_);
    copied_mst_graph->mst_weight_ = mst_weight_;
    copied_mst_graph->MST_edges_by_id_ = MST_edges_by_id_;
    std::vector<int> keys = getMapKeys<int, Node*>(copied_mst_graph->map_id_nodes_);
    for(std::pair<int, int> edge_by_id : MST_edges_by_id_) {
        MSTNode* endpoint1 = (MSTNode*) copied_mst_graph->map_id_nodes_[edge_by_id.first];
        MSTNode* endpoint2 = (MSTNode*) copied_mst_graph->map_id_nodes_[edge_by_id.second];
        Edge* edge = new Edge(endpoint1, endpoint2);
        copied_mst_graph->MST_edges_.push_back(edge);
        endpoint1->addMSTEdge(edge);
        endpoint2->addMSTEdge(edge);
    }
    // Add copy communication edges later if needed
    return copied_mst_graph;
}

MSTGraph::~MSTGraph() {
    std::cout << getGraphTypeStr() << "'s Destructor\n";
    for(Edge* edge : MST_edges_) delete edge;
    for(Edge* edge : communication_edges_) delete edge;
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