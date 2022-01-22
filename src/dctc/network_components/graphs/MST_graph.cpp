#include <cassert>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include "data_structures/counter.h"
#include "data_structures/union_find.h"
#include "dctc/utils.h"
#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/graphs/MST_graph.h"
#include "dctc/algorithms/connectivity/communication_checker.h"


MSTGraph::MSTGraph() {}

void MSTGraph::init(const std::vector<Node*>& nodes, GraphNodeType graph_node_type) {
    graph_node_type_ = graph_node_type;
    for(Node* node : nodes) {
        MSTNode* mst_node = (MSTNode*) NodeFactory::createNode(node, graph_node_type_);
        nodes_.push_back(mst_node);
        int id = mst_node->getId();
        map_id_nodes_[id] = mst_node;
        node_ids_.push_back(id);
    }
    n_nodes_ = nodes.size();
}

MSTGraph::MSTGraph(const std::vector<Node*>& nodes, GraphNodeType graph_node_type, bool build_MST) {
    init(nodes, graph_node_type);
    if (build_MST) mst_weight_ = buildMST();
}

GraphNodeType MSTGraph::getGraphNodeType() const {return graph_node_type_;}

std::string MSTGraph::getGraphTypeStr() const {return "MSTGraph";}

int MSTGraph::getNNodes() const {return n_nodes_;}

std::vector<Node*> MSTGraph::getNodes() const {return nodes_;}

std::vector<Edge*> MSTGraph::getMSTEdges() const {return MST_edges_;}

MSTNode* MSTGraph::rootTree(MSTNode* root_node) {
    for(Node* node : nodes_) {
        MSTNode* MST_node = (MSTNode*) node;
        MST_node->level_in_rooted_mst_ = 0;
        MST_node->MST_children_edges_ = std::vector<Edge*>();
        MST_node->MST_children_nodes_ = std::vector<MSTNode*>();
        MST_node->MST_children_edges_map_ = std::unordered_map<MSTNode*, Edge*>();
    }
    dfs1(root_node, nullptr, 0);
    return root_node;
}

void MSTGraph::dfs1(MSTNode* u, MSTNode* par, int level) {
    if (u == nullptr) return;
    max_level_ = std::max(max_level_, level);
    u->level_in_rooted_mst_ = level;
    u->parent_in_rooted_MST_ = par;
    if (par != nullptr) u->edge_to_parent_in_rooted_MST_ = u->mst_edges_[par];
    for(auto edge : u->mst_edges_) {
        MSTNode* v = edge.first;
        if (v == par) continue;
        u->MST_children_edges_map_[v] = edge.second;
        dfs1(v, u, level + 1);
    }
    u->MST_children_nodes_ = getMapKeys<MSTNode*, Edge*>(u->MST_children_edges_map_);
    u->MST_children_edges_ = getMapValues<MSTNode*, Edge*>(u->MST_children_edges_map_);
}


Edge* MSTGraph::addCommunicationEdge(MSTNode* a, MSTNode* b) {
    //std::cout << communication_edges_.size() << ": Add communication edge (" << a->getId() << "," << b->getId() << ")\n";
    Edge* edge = new Edge(a, b);
    a->addCommunicationEdge(edge);
    b->addCommunicationEdge(edge);
    if (!CommunicationChecker::checkAngleOneNode1(a)) std::cout << "failed angle " << *a << ' ' << *b << '\n';
    if (!CommunicationChecker::checkAngleOneNode1(b)) std::cout << "failed angle " << *b << ' ' << *a << '\n';
    assert(CommunicationChecker::checkAngleOneNode1(a));
    assert(CommunicationChecker::checkAngleOneNode1(b));
    communication_edges_.push_back(edge);
    return edge;
}

std::vector<Edge*> MSTGraph::getCommunicationEdges() const {return communication_edges_;}

double MSTGraph::buildMST() {
    std::vector<Edge*> edges;
    for(int i : node_ids_) {
        for(int j : node_ids_) {
            if (j <= i) continue;
            Edge* edge = new Edge(map_id_nodes_[i], map_id_nodes_[j]);
            edges.push_back(edge);
        }
    }
    return buildMST(edges, true);
}

double MSTGraph::buildMST(std::vector<Edge*>& edges, bool delete_edge) {
    time_t start_time = time(NULL);
    UnionFind union_find(Counter::get());
    double mst_weight = 0;
    int n_mst_edges = 0;
    std::sort(edges.begin(), edges.end(),
              [](Edge* a, Edge* b) {return a->length() < b->length();});

    for(Edge* edge : edges) {
        if (n_mst_edges < n_nodes_ - 1) {
            int fr = edge->getEndpoint1()->getId();
            int to = edge->getEndpoint2()->getId();
            if (union_find.find(fr) == union_find.find(to)) continue;
            union_find.join(fr, to);
            mst_weight += edge->length();
            ++n_mst_edges;
            Edge* mst_edge = new Edge(map_id_nodes_[fr], map_id_nodes_[to]);

            assert(map_id_nodes_[fr] != nullptr);
            assert(map_id_nodes_[to] != nullptr);

            MST_edges_.push_back(mst_edge);
            MSTNode* endpoint1 = (MSTNode*) mst_edge->getEndpoint1();
            endpoint1->addMSTEdge(mst_edge);
            MSTNode* endpoint2 = (MSTNode*) mst_edge->getEndpoint2();
            endpoint2->addMSTEdge(mst_edge);
            MST_edges_by_id_.push_back({fr, to});
        }
        if (delete_edge) delete edge;
    }

    MSTNode* root = nullptr;
    for(Node* node : nodes_) {
        MSTNode* mst_node = (MSTNode*) node;
        mst_node->setIsLeaf(mst_node->getMSTEdgeAdjNodes().size() == 1);
        if (root == nullptr && mst_node->is_leaf_) root = mst_node;
    } 

    rootTree(root);

    time_t end_time = time(NULL);
    std::cout << "Done build MST in " << end_time - start_time << "s\n";
    return mst_weight;
}

MSTGraph* MSTGraph::cloneFrom(const MSTGraph* mst_graph, GraphNodeType graph_node_type) {
    init(mst_graph->nodes_, graph_node_type);
    mst_weight_ = mst_graph->mst_weight_;
    MST_edges_by_id_ = mst_graph->MST_edges_by_id_;
    for(std::pair<int, int> edge_by_id : MST_edges_by_id_) {
        MSTNode* endpoint1 = (MSTNode*) map_id_nodes_[edge_by_id.first];
        MSTNode* endpoint2 = (MSTNode*) map_id_nodes_[edge_by_id.second];
        Edge* edge = new Edge(endpoint1, endpoint2);
        MST_edges_.push_back(edge);
        endpoint1->addMSTEdge(edge);
        endpoint2->addMSTEdge(edge);
    }
    // Add copy communication edges later if needed
    return this;
}

MSTGraph* MSTGraph::deepCopy(GraphNodeType graph_node_type) const {
    MSTGraph* copied_mst_graph = new MSTGraph();
    return copied_mst_graph->cloneFrom(this, graph_node_type);
}

MSTGraph::~MSTGraph() {
    //std::cout << getGraphTypeStr() << "'s Destructor\n";
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