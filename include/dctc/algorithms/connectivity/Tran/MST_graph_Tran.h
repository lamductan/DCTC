#ifndef DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_

#include <set>
#include "dctc/network_components/graphs/MST_graph.h"
#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"


class MSTGraphTran : public MSTGraph {
protected:
    std::vector<GroupTran*> groups_;
    std::unordered_map<int, GroupTran*> map_id_groups_;
    std::unordered_map<int, std::vector<int>> group_neighbors_;
    std::set<int, Node*> leaves_at_levels_;
    MSTNode* root;

public:
    MSTGraphTran(const MSTGraph* MST_graph);

    MSTNode* findRootNode();
    MSTNode* rootTree(MSTNode* root);
    void initLeavesSet();
    void partition();
    void establishCommunicationGraph();
    void doAllSteps();

    virtual ~MSTGraphTran();
    friend std::ostream& operator<<(std::ostream& os, const MSTGraphTran& MST_graph);
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_GRAPH_TRAN_H_