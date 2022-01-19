#ifndef DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "dctc/network_components/nodes/MST_node.h"


struct GroupTran {
    int id_;
    std::vector<Node*> core_nodes_;
    std::vector<Node*> non_core_nodes_;
};

class MSTGraphTran;

class MSTNodeTran : public MSTNode {
friend class MSTGraphTran;
protected:
    GroupTran* group_;
    int level_in_rooted_mst_;
    std::unordered_map<Node*, Edge*> MST_children_edges_;
    Node* parent_in_rooted_MST_;
    Edge* edge_to_parent_in_rooted_MST_;

public:
    MSTNodeTran(MSTNode* node);
};


#endif //DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_