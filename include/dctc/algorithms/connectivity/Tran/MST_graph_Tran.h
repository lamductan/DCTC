#ifndef DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_

#include <set>
#include <unordered_set>

#include "dctc/network_components/graphs/MST_graph.h"
#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"


class MSTGraphTran : public MSTGraph {
protected:
    std::unordered_map<int, std::unordered_set<MSTNodeTran*>> leaves_at_levels_;

    std::vector<MSTNodeTran*> getRemainedNodesInSubtree(MSTNodeTran* u) const;
    void dfs2(MSTNodeTran* u, std::vector<MSTNodeTran*>& remained_nodes_in_subtree) const;
    void removeNode(MSTNodeTran* node, bool in_group);
    GroupTran* formGroup(std::vector<MSTNodeTran*>& nodes);
    void printGroup();

    std::vector<GroupTran*> groups_;
    std::vector<MSTNodeTran*> group_roots_;
    std::vector<MSTNodeTran*> remained_nodes_;
    std::unordered_map<int, GroupTran*> map_id_groups_;
    std::set<std::pair<int, int>> group_neighbor_relations_;
    MSTNodeTran* root_;

public:
    MSTGraphTran(const MSTGraph* MST_graph);

    MSTNodeTran* findRootNode();
    void partition();
    void findGroupNeighborRelations();
    void establishCommunicationGraph();
    MSTGraph* doAllSteps();

    virtual ~MSTGraphTran();
    friend std::ostream& operator<<(std::ostream& os, const MSTGraphTran& MST_graph);
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_