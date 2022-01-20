#include <cassert>
#include <algorithm>

#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"


MSTGraphTran::MSTGraphTran(const MSTGraph* MST_graph) {
    MSTGraph::cloneFrom(MST_graph, MST_GRAPH_NODE_TRAN);
}

MSTNodeTran* MSTGraphTran::findRootNode() {
    for(Node* node : nodes_) {
        std::vector<Node*> adj_nodes = dynamic_cast<MSTNode*>(node)->getMSTEdgeAdjNodes();
        int n_adj_nodes = adj_nodes.size();
        int n_adj_leaf_nodes = 0;
        for(Node* adj_node : adj_nodes)
            n_adj_leaf_nodes += dynamic_cast<MSTNode*>(adj_node)->isLeaf();
        int n_adj_non_leaf_nodes = n_adj_nodes - n_adj_leaf_nodes;
        if (n_adj_leaf_nodes >= 1 && n_adj_non_leaf_nodes <= 1)
            return root_ = (MSTNodeTran*) node;
    }
    return nullptr;
}

MSTNodeTran* MSTGraphTran::rootTree(MSTNodeTran* root_node) {
    std::unordered_map<MSTNodeTran*, bool> vis;
    dfs1(root_node, nullptr, 0, vis);
    return nullptr;
}

void MSTGraphTran::dfs1(MSTNodeTran* u, MSTNodeTran* par, int level, std::unordered_map<MSTNodeTran*, bool>& vis) {
    if (u == nullptr) return;
    if (vis[u]) return;
    vis[u] = true;
    max_level_ = std::max(max_level_, level);
    u->level_in_rooted_mst_ = level;
    u->parent_in_rooted_MST_ = par;
    u->edge_to_parent_in_rooted_MST_ = u->mst_edges_[par];
    for(auto edge : u->mst_edges_) {
        MSTNodeTran* v = (MSTNodeTran*) edge.first;
        if (v == par) continue;
        u->MST_children_edges_map_[v] = edge.second;
        dfs1(v, u, level + 1, vis);
    }
    u->MST_children_nodes_ = getMapKeys<MSTNodeTran*, Edge*>(u->MST_children_edges_map_);
    u->MST_children_edges_ = getMapValues<MSTNodeTran*, Edge*>(u->MST_children_edges_map_);
}

void MSTGraphTran::partition() {
    for(Node* node : nodes_) {
        MSTNodeTran* mst_node_tran = (MSTNodeTran*) node;
        int level = mst_node_tran->level_in_rooted_mst_;
        leaves_at_levels_[level].insert(mst_node_tran);
    }

    for(int level = max_level_; level >= 0; --level) {
        while (!leaves_at_levels_[level].empty()) {
            MSTNodeTran* node = *(leaves_at_levels_[level].begin());
            MSTNodeTran* cur = node;
            bool can_form_group = false;
            // try parent, grandparent and greatparent 's subtree
            for(int i = 0; i < 3; ++i) {
                if (cur == nullptr) break;
                MSTNodeTran* par = cur->parent_in_rooted_MST_;
                if (par == nullptr) break;
                std::vector<MSTNodeTran*> remained_nodes_in_subtree_of_par = getRemainedNodesInSubtree(par);
                if (remained_nodes_in_subtree_of_par.size() >= 4) {
                    can_form_group = true;
                    GroupTran* group = formGroup(remained_nodes_in_subtree_of_par);
                    groups_.push_back(group);
                    break;
                } else {
                    cur = par;
                }
            }
            if (!can_form_group) {
                removeNode(node, false);
                remained_nodes_.push_back(node);
            }
        }
    }
}

std::vector<MSTNodeTran*> MSTGraphTran::getRemainedNodesInSubtree(MSTNodeTran* u) const {
    std::vector<MSTNodeTran*> remained_nodes_in_subtree;
    dfs2(u, remained_nodes_in_subtree);
    return remained_nodes_in_subtree;
}

void MSTGraphTran::dfs2(MSTNodeTran* u, std::vector<MSTNodeTran*>& remained_nodes_in_subtree) const {
    if (u == nullptr) return;
    remained_nodes_in_subtree.push_back(u);
    for(MSTNodeTran* child : u->MST_children_nodes_) {
        if (!child->removed_) dfs2(child, remained_nodes_in_subtree);
    }
}

void MSTGraphTran::removeNode(MSTNodeTran* node, bool in_group) {
    node->removed_ = true;
    node->in_group_ = in_group;
    leaves_at_levels_[node->level_in_rooted_mst_].erase(node);
}

GroupTran* MSTGraphTran::formGroup(std::vector<MSTNodeTran*>& nodes) {
    GroupTran* group = new GroupTran;
    group->group_id_ = groups_.size();
    std::sort(nodes.begin(), nodes.end(),
              [](MSTNodeTran* a, MSTNodeTran* b) {return a->level_in_rooted_mst_ < b->level_in_rooted_mst_;}
    );
    MSTNodeTran* group_root = nodes[0];
    group->group_root_ = group_root;
    group_roots_.push_back(group_root);

    for(int i = 0; i < 4; ++i) {
        MSTNodeTran* node = nodes[i];
        node->group_ = group;
        group->core_nodes_.push_back(node);
        removeNode(node, true);
    }
    for(int i = 4; i < nodes.size(); ++i) {
        MSTNodeTran* node = nodes[i];
        node->group_ = group;
        group->non_core_nodes_.push_back(node);
        removeNode(node, true);
    }

    return group;
}

void MSTGraphTran::findGroupNeighborRelations() {
    for(int i = 0; i < groups_.size(); ++i) {
        GroupTran* group_i = groups_[i];
        MSTNodeTran* group_i_root = group_i->group_root_;
        MSTNodeTran* group_i_root_par = group_i_root->parent_in_rooted_MST_;
        if (group_i_root_par == nullptr) continue;
        GroupTran* group_i_root_par_group = group_i_root_par->group_;
        if (group_i_root_par_group == nullptr) continue;
        int par_group_id = group_i_root_par_group->group_id_;
        int j = i;
        if (j < par_group_id) std::swap(j, par_group_id);
        std::pair<int, int> pair_group = {j, par_group_id};
        group_neighbor_relations_.insert(pair_group);
    }
}

void MSTGraphTran::printGroup() {
    std::cout << "Groups{\n";
    std::string tab = "    ";
    for(GroupTran* group : groups_) {
        std::cout << tab << "Group_" << group->group_id_ << "[";
        std::cout << "root(" << group->group_root_->getId() << ");";

        std::cout << "core(";
        for(Node* core_node : group->core_nodes_)
            std::cout << core_node->getId() << ",";
        std::cout << ");";

        std::cout << "non-core(";
        for(MSTNodeTran* non_core_node : group->non_core_nodes_)
            std::cout << non_core_node->getId() << ",";
        std::cout << ")";
        std::cout << "]\n";
    }
    std::cout << "}\n";
    std::cout << "Group Neighbors Relation:\n" << tab;
    for(const std::pair<int, int>& group_neighbor_relation : group_neighbor_relations_) {
        std::cout << "[" << group_neighbor_relation.first << ',' << group_neighbor_relation.second << "],";
    }
    std::cout << "\n";
}

void MSTGraphTran::establishCommunicationGraph() {
    //orient and connect inside each group
    std::cout << "\nOrient and connect inside each group\n";
    for(GroupTran* group : groups_) {
        //Connect core nodes
        std::vector<Node*> core_nodes_after_orientation = orientFourNodesPI_2CoverPlane(group->core_nodes_);
        Node *a = core_nodes_after_orientation[0], *b = core_nodes_after_orientation[1];
        Node *c = core_nodes_after_orientation[2], *d = core_nodes_after_orientation[3];
        addCommunicationEdge((MSTNode*) a, (MSTNode*) b);
        addCommunicationEdge((MSTNode*) a, (MSTNode*) c);
        addCommunicationEdge((MSTNode*) b, (MSTNode*) d);
        //Connect non-core nodes to core nodes
        for(MSTNodeTran* non_core_node : group->non_core_nodes_) {
            Point2D non_core_node_position = non_core_node->getPoint2D();
            for(Node* core_node : core_nodes_after_orientation) {
                if (dynamic_cast<Sector*>(core_node->getCommunicationAntenna())->containsPoint2DWithInfRange(
                    non_core_node_position))
                {
                    Point2D core_node_position = core_node->getPoint2D();
                    dynamic_cast<Sector*>(non_core_node->getCommunicationAntenna())->orientToCoverPoint2D(
                        core_node_position);
                    addCommunicationEdge((MSTNode*) core_node, non_core_node);
                }
            }
        }
    }

    //intergroup orientation and connection
    std::cout << "\nIntergroup orientation and connection\n";
    for(const std::pair<int, int>& group_neighbor_relation : group_neighbor_relations_) {
        int i = group_neighbor_relation.first;
        int j = group_neighbor_relation.second;
        //std::cout << "Connect group " << i << " with group " << j << '\n';
        GroupTran* group_i = groups_[i];
        GroupTran* group_j = groups_[j];
        bool done_connect_i_j = false;
        for(Node* core_node_i : group_i->core_nodes_) {
            for(Node* core_node_j : group_j->core_nodes_) {
                if (core_node_i->canCoverOtherNodeByCommunicationAntenna(core_node_j)
                    && core_node_j->canCoverOtherNodeByCommunicationAntenna(core_node_i))
                {
                    addCommunicationEdge((MSTNode*) core_node_i, (MSTNode*) core_node_j);
                    done_connect_i_j = true;
                    break;
                }
            }
            if (done_connect_i_j) break;
        }
        assert(done_connect_i_j);
    }

    //orient and connect remain nodes
    std::cout << "\nOrient and connect remain nodes\n";
    for(MSTNode* remained_node : remained_nodes_) {
        Node* node_to_connect = nullptr;
        double min_distance = INT_MAX;
        Point2D node_position = remained_node->getPoint2D();
        for(Node* node : nodes_) {
            if (node == remained_node) continue;
            MSTNodeTran* mst_node = (MSTNodeTran*) node;
            if (!mst_node->in_group_) continue;
            double distance = computeEuclidDistance(node_position, mst_node->getPoint2D());
            if (distance < min_distance) {
                min_distance = distance;
                node_to_connect = mst_node;
            }
        }
        assert(node_to_connect != nullptr);
        dynamic_cast<Sector*>(remained_node->getCommunicationAntenna())->orientToCoverPoint2D(
            node_to_connect->getPoint2D());
        addCommunicationEdge((MSTNode*) remained_node, (MSTNode*) node_to_connect);
    }
}

void MSTGraphTran::doAllSteps() {
    root_ = findRootNode();
    std::cout << "MSTNodeTran root = " << *root_ << '\n';
    rootTree(root_);
    //std::cout << * (MSTGraph*) this << '\n';
    partition();
    findGroupNeighborRelations();
    //printGroup();
    establishCommunicationGraph();
    //std::cout << * (MSTGraph*) this << '\n';
}

MSTGraphTran::~MSTGraphTran() {
    for(GroupTran* group : groups_) delete group;
}