#include <cassert>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <fstream>

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
    if (build_MST) {
        mst_weight_ = buildMST();
        n_total_nodes_omni_ = nodes_.size();
        for(Edge* edge :MST_edges_) {
            n_total_nodes_omni_ += calculateNumRelaysOmni(edge, nodes_[0]->getRC());
        }
    }
}

int MSTGraph::calculateNumRelaysOmni(Edge* edge, long double r_c) {
    return ceil(edge->length() / r_c) - 1;
}

GraphNodeType MSTGraph::getGraphNodeType() const {return graph_node_type_;}

std::string MSTGraph::getGraphTypeStr() const {return "MSTGraph";}

int MSTGraph::getNNodes() const {return n_nodes_;}

std::vector<Node*> MSTGraph::getNodes() const {return nodes_;}

std::vector<Edge*> MSTGraph::getMSTEdges() const {return MST_edges_;}

long double MSTGraph::getMinimumMSTEdgeLength() const {
    long double min_MST_edge_length = INT_MAX;
    for(Edge* edge : MST_edges_)
        min_MST_edge_length = std::min(min_MST_edge_length, edge->length());
    return min_MST_edge_length;
}

long double MSTGraph::getMaximumMSTEdgeLength() const {
    long double max_MST_edge_length = 0;
    for(Edge* edge : MST_edges_)
        max_MST_edge_length = std::max(max_MST_edge_length, edge->length());
    return max_MST_edge_length;
}

long double MSTGraph::getMinimumCommunicationEdgeLength() const {
    long double min_communication_edge_length = INT_MAX;
    for(Edge* edge : communication_edges_)
        min_communication_edge_length = std::min(min_communication_edge_length, edge->length());
    return min_communication_edge_length;
}

long double MSTGraph::getMaximumCommunicationEdgeLength() const {
    long double max_communication_edge_length = 0;
    for(Edge* edge : communication_edges_)
        max_communication_edge_length = std::max(max_communication_edge_length, edge->length());
    return max_communication_edge_length;
}

int MSTGraph::getNTotalNodesOmni() const {return n_total_nodes_omni_;}

void MSTGraph::setNTotalNodesOmni(int n_total_nodes_omni) {n_total_nodes_omni_ = n_total_nodes_omni;}

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

long double MSTGraph::buildMST() {
    std::vector<Edge*> edges;
    for(int i : node_ids_) {
        for(int j : node_ids_) {
            if (j <= i) continue;
            Edge* edge = new Edge(map_id_nodes_[i], map_id_nodes_[j]);
            edges.push_back(edge);
        }
    }
    mst_weight_ = buildMST(edges);
    for(Edge* edge : edges) delete edge;
    return mst_weight_;
}

long double MSTGraph::buildMST(std::vector<Edge*>& edges) {
    time_t start_time = time(NULL);
    UnionFind union_find(Counter::get());
    long double mst_weight = 0;
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
        } else {
            break;
        }
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
    n_total_nodes_omni_ = mst_graph->n_total_nodes_omni_;
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

Edge* MSTGraph::orientAndConnect(MSTNode* node_to_orient, MSTNode* node_to_connect) {
    dynamic_cast<Sector*>(node_to_orient->getCommunicationAntenna())->orientToCoverPoint2D(
        node_to_connect->getPoint2D(), false);
    return addCommunicationEdge(node_to_orient, node_to_connect);
}

void MSTGraph::save(const std::string& save_path) {
    std::ofstream fout;
    fout.open(save_path);
    fout << nodes_.size() << '\n';
    for(Node* node : nodes_) {
        fout << node->getX() << ' ' << node->getY() << ' ';
        fout << node->getSensingSector()->getRadius() << ' ' << node->getSensingSector()->getAngle() << ' ';
        fout << node->getNodeType() << '\n';
    }
    fout << MST_edges_.size() << '\n';
    for(Edge* edge : MST_edges_) {
        fout << edge->getEndpoint1()->getX() << ' ' << edge->getEndpoint1()->getY() << ' ';
        fout << edge->getEndpoint2()->getX() << ' ' << edge->getEndpoint2()->getY() << '\n';
    }
    fout.close();
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