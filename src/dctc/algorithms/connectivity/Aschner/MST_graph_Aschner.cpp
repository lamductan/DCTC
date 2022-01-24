#include <cassert>
#include <ctime>
#include <algorithm>
#include <iomanip>

#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/utils.h"
#include "dctc/algorithms/connectivity/Aschner/MST_graph_Aschner.h"
#include "dctc/algorithms/connectivity/communication_checker.h"


MSTGraphAschner::MSTGraphAschner(const MSTGraph* MST_graph) {
    MSTGraph::cloneFrom(MST_graph);
}

void MSTGraphAschner::dfs_preorder(Node* u, Node* par, std::vector<Node*>& nodes_in_preorder) const {
    if (u == nullptr) return;
    nodes_in_preorder.push_back(u);
    for(Node* child : dynamic_cast<MSTNode*>(u)->getMSTEdgeAdjNodes()) {
        if (child == par) continue;
        dfs_preorder(child, u, nodes_in_preorder);
    }
}

void MSTGraphAschner::partition() {
    std::vector<Node*> nodes_in_preorder;
    int n = nodes_.size();
    dfs_preorder(nodes_[0], nullptr, nodes_in_preorder);
    for(int i = 0; i < n; i += 8) {
        std::vector<Node*> group;
        if (i + 8 < n) {
            for(int j = i; j < i + 8; ++j) {
                Node* node = nodes_in_preorder[j];
                group.push_back(node);
                nodes_oriented_[node] = true;
            }
            std::sort(group.begin(), group.end(),
                      [](Node* a, Node* b) {return a->getX() < b->getX();}
            );
        } else {
            for(int j = i; j < n; ++j)
                group.push_back(nodes_in_preorder[j]);
        }
        groups_.push_back(group);
    }
}

void MSTGraphAschner::establishCommunicationGraph() {
    //orient and connect inside each group
    std::cout << "\nOrient and connect inside each group\n";
    for(std::vector<Node*>& group : groups_) {
        if (group.size() < 8) continue;
        //Connect nodes in each subgroup
        std::vector<Node*> subgroup0{group.begin(), group.begin() + 4};
        std::vector<Node*> subgroup1{group.begin() + 4, group.end()};
        std::vector<std::vector<Node*>> subgroups{subgroup0, subgroup1};
        for(int i = 0; i < 2; ++i) {
            std::vector<Node*> sub_group_after_orientation = orientFourNodesPI_2CoverPlane(subgroups[i]);
            Node *a = sub_group_after_orientation[0], *b = sub_group_after_orientation[1];
            Node *c = sub_group_after_orientation[2], *d = sub_group_after_orientation[3];
            addCommunicationEdge((MSTNode*) a, (MSTNode*) b);
            addCommunicationEdge((MSTNode*) a, (MSTNode*) c);
            addCommunicationEdge((MSTNode*) b, (MSTNode*) d);
        }
        //Connect two subgroups
        for(Node* node_in_subgroup_0 : subgroup0) {
            for(Node* node_in_subgroup_1 : subgroup1) {
                if (node_in_subgroup_0->canCoverOtherNodeByCommunicationAntenna(node_in_subgroup_1)
                    && node_in_subgroup_1->canCoverOtherNodeByCommunicationAntenna(node_in_subgroup_0))
                {
                    addCommunicationEdge((MSTNode*) node_in_subgroup_0, (MSTNode*) node_in_subgroup_1);
                }
            }
        }
    }

    //intergroup orientation and connection
    std::cout << "\nIntergroup orientation and connection\n";
    for(int i = 1; i < groups_.size(); ++i) {
        if (groups_[i].size() < 8) continue;
        for(Node* node_this_group : groups_[i]) {
            for(Node* node_prev_group : groups_[i - 1]) {
                if (node_this_group->canCoverOtherNodeByCommunicationAntenna(node_prev_group)
                    && node_prev_group->canCoverOtherNodeByCommunicationAntenna(node_this_group))
                {
                    addCommunicationEdge((MSTNode*) node_this_group, (MSTNode*) node_prev_group);
                }
            }
        }
    }

    if (groups_.back().size() < 8) {
        //orient and connect remain nodes
        std::cout << "\nOrient and connect remain nodes\n";
        for(Node* remained_node : groups_.back()) {
            Node* node_to_connect = findBestNodeToConnect(remained_node, nodes_);
            assert(node_to_connect != nullptr);
            assert(dynamic_cast<Sector*>(node_to_connect->getCommunicationAntenna())->containsPoint2DWithInfRange(
                remained_node->getPoint2D()));
            orientAndConnect((MSTNode*) remained_node, (MSTNode*) node_to_connect);
        }
    }
}

Node* MSTGraphAschner::findBestNodeToConnect(Node* node, const std::vector<Node*>& candidates) {
    MSTNode* node_to_connect = nullptr;
    long double min_distance = INT_MAX;
    Point2D node_position = node->getPoint2D();
    for(Node* candidate : candidates) {
        if (candidate == node) continue;
        if (!nodes_oriented_[candidate]) continue;
        MSTNode* mst_candidate = (MSTNode*) candidate;
        if (!mst_candidate->canCoverOtherNodeByCommunicationAntenna(node)) continue;
        long double distance = computeEuclidDistance(node_position, mst_candidate->getPoint2D());
        if (distance < min_distance) {
            min_distance = distance;
            node_to_connect = mst_candidate;
        }
    }
    return node_to_connect;
}

MSTGraph* MSTGraphAschner::doAllSteps() {
    time_t start_time = time(NULL);

    //Step 1: Partition
    partition();

    //Step 2: Establish communication graph
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
    std::cout << "Result: OK. Returned MST Graph satisfies both symmetric connectivity and angle requirements.\n";

    return result_MST_graph;
}