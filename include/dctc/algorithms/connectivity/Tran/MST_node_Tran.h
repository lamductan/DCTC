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
    bool removed_ = false;
    bool in_group_ = false;

    virtual std::string getStringContent() const override;

public:
    MSTNodeTran(MSTNode* node);
    virtual std::string getNodeTypeStr() const override;
    virtual ~MSTNodeTran();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_TRAN_MST_NODE_TRAN_H_