#ifndef DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "dctc/network_components/nodes/MST_node.h"


class MSTNodeTran;

struct GroupTran {
    int group_id_;
    MSTNodeTran* group_root_;
    std::vector<Node*> core_nodes_;
    std::vector<MSTNodeTran*> non_core_nodes_;
};

class MSTGraphTran;

class MSTNodeTran : public MSTNode {
friend class MSTGraphTran;
protected:
    GroupTran* group_ = nullptr;
    int level_in_rooted_mst_;
    std::unordered_map<MSTNodeTran*, Edge*> MST_children_edges_map_;
    std::vector<MSTNodeTran*> MST_children_nodes_;
    std::vector<Edge*> MST_children_edges_;

    MSTNodeTran* parent_in_rooted_MST_ = nullptr;
    Edge* edge_to_parent_in_rooted_MST_;
    bool removed_ = false;
    bool in_group_ = false;

    virtual std::string getStringContent() const override;

public:
    MSTNodeTran(MSTNode* node);
    virtual std::string getNodeTypeStr() const override;
    std::vector<MSTNodeTran*> getMSTChildrenNodes() const;
    std::vector<Edge*> getMSTChildrenEdges() const;
    std::unordered_map<MSTNodeTran*, Edge*> getMSTChildrenEdgesMap() const;
    virtual ~MSTNodeTran();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_