#include <cassert>
#include <ctime>
#include <algorithm>
#include <iomanip>

#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/algorithms/connectivity/Tran/MST_graph_Tran.h"
#include "dctc/algorithms/connectivity/communication_checker.h"


MSTGraphTran::MSTGraphTran(const MSTGraph* MST_graph) {
    MSTGraph::cloneFrom(MST_graph, MST_GRAPH_NODE_TRAN);
}

MSTNodeTran* MSTGraphTran::findRootNode() {
    for(Node* node : nodes_) {
        std::vector<MSTNode*> adj_nodes = dynamic_cast<MSTNode*>(node)->getMSTEdgeAdjNodes();
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

void MSTGraphTran::partition() {
    for(Node* node : nodes_) {
        MSTNodeTran* mst_node_tran = (MSTNodeTran*) node;
        int level = mst_node_tran->level_in_rooted_mst_;
        nodes_at_levels_[level].insert(mst_node_tran);
    }

    for(int level = max_level_; level >= 0; --level) {
        while (!nodes_at_levels_[level].empty()) {
            MSTNodeTran* node = *(nodes_at_levels_[level].begin());
            MSTNodeTran* cur = node;
            bool can_form_group = false;
            // try parent, grandparent and greatparent 's subtree
            for(int i = 0; i < 3; ++i) {
                if (cur == nullptr) break;
                MSTNodeTran* par = (MSTNodeTran*) cur->parent_in_rooted_MST_;
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
    for(MSTNode* child : u->MST_children_nodes_) {
        MSTNodeTran* mst_node_Tran_child = (MSTNodeTran*) child;
        if (!mst_node_Tran_child->removed_) dfs2(mst_node_Tran_child, remained_nodes_in_subtree);
    }
}

void MSTGraphTran::removeNode(MSTNodeTran* node, bool in_group) {
    node->removed_ = true;
    node->in_group_ = in_group;
    nodes_at_levels_[node->level_in_rooted_mst_].erase(node);
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
        MSTNodeTran* group_i_root_par = (MSTNodeTran*) group_i_root->parent_in_rooted_MST_;
        if (group_i_root_par == nullptr) continue;
        GroupTran* group_i_root_par_group = group_i_root_par->group_;
        if (group_i_root_par_group == nullptr) continue;
        int par_group_id = group_i_root_par_group->group_id_;
        int j = i;
        if (j < par_group_id) std::swap(j, par_group_id);
        std::pair<int, int> pair_group = {j, par_group_id};
        group_neighbor_relations_.insert(pair_group);
    }

    std::vector<int> groups_adj_remained_nodes;
    for(MSTNode* remained_node : remained_nodes_) {
        for(MSTNode* adj_node : remained_node->getMSTEdgeAdjNodes()) {
            MSTNodeTran* adj_node_1 = (MSTNodeTran*) adj_node;
            if (adj_node_1->in_group_) groups_adj_remained_nodes.push_back(adj_node_1->group_->group_id_);
        }
    }
    for(int i : groups_adj_remained_nodes) {
        for(int j : groups_adj_remained_nodes) {
            if (i <= j) continue;
            group_neighbor_relations_.insert({i, j});
        }
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
            MSTNode* core_node_to_connect = findBestNodeToConnect(non_core_node, group->core_nodes_);
            assert(core_node_to_connect != nullptr);
            orientAndConnect(non_core_node, core_node_to_connect);
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
                }
            }
        }
        assert(done_connect_i_j);
    }

    //orient and connect remain nodes
    std::cout << "\nOrient and connect remain nodes\n";
    for(MSTNode* remained_node : remained_nodes_) {
        MSTNode* node_to_connect = findBestNodeToConnect(remained_node, nodes_);
        assert(node_to_connect != nullptr);
        assert(dynamic_cast<Sector*>(node_to_connect->getCommunicationAntenna())->containsPoint2DWithInfRange(
            remained_node->getPoint2D()));
        orientAndConnect(remained_node, (MSTNode*) node_to_connect);
    }
}

MSTNode* MSTGraphTran::findBestNodeToConnect(MSTNode* node, const std::vector<Node*>& candidates) const {
    MSTNode* node_to_connect = nullptr;
    long double min_distance = INT_MAX;
    Point2D node_position = node->getPoint2D();
    for(Node* candidate : candidates) {
        if (candidate == node) continue;
        MSTNodeTran* mst_candidate = (MSTNodeTran*) candidate;
        if (!mst_candidate->in_group_) continue;
        if (!mst_candidate->canCoverOtherNodeByCommunicationAntenna(node)) continue;
        long double distance = computeEuclidDistance(node_position, mst_candidate->getPoint2D());
        if (distance < min_distance) {
            min_distance = distance;
            node_to_connect = mst_candidate;
        }
    }
    return node_to_connect;
}

MSTGraph* MSTGraphTran::doAllSteps() {
    time_t start_time = time(NULL);

    //Step 1: Find root node
    root_ = findRootNode();
    //std::cout << "MSTNodeTran root = " << *root_ << '\n';

    //Step 2: Root tree
    rootTree(root_);
    //std::cout << *(MSTGraph*) this << '\n';

    //Step 3: Partition
    partition();

    //Step 4: Find group neighbor relations
    findGroupNeighborRelations();
    //printGroup();

    //Step 5: Establish communication graph
    establishCommunicationGraph();
    //std::cout << * (MSTGraph*) this << '\n';
    assert(CommunicationChecker::checkConnectivity1(this));

    //Step 6: Return MST of the communication graph
    MSTGraph* result_MST_graph = new MSTGraph(nodes_, MST_GRAPH_NODE, false);
    result_MST_graph->buildMST(communication_edges_);
    //std::cout << "result_MST_graph:\n";
    //std::cout << *result_MST_graph << '\n';

    //Check result
    assert(CommunicationChecker::checkConnectivity(result_MST_graph));
    assert(CommunicationChecker::checkAngle(result_MST_graph));

    time_t end_time = time(NULL);
    std::cout << "Done " << __PRETTY_FUNCTION__ << " in " << end_time - start_time << "s\n";
    std::cout << "Result: OK. Returned MST Graph satisfies both connectivity and angle requirements.\n";

    return result_MST_graph;
}

MSTGraphTran::~MSTGraphTran() {
    for(GroupTran* group : groups_) delete group;
}