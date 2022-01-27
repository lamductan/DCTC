#ifndef DCTC_ALGORITHMS_CONNECTIVITY_LAM_MST_NODE_LAM_H_
#define DCTC_ALGORITHMS_CONNECTIVITY_LAM_MST_NODE_LAM_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "dctc/network_components/nodes/MST_node.h"


class LongShortEdgeRelaysAlg;
class LongEdgeFirstRelaysAlg;
class ShortEdgeFirstRelaysAlg;


class MSTNodeLam : public MSTNode {
friend class LongShortEdgeRelaysAlg;
friend class LongEdgeFirstRelaysAlg;
friend class ShortEdgeFirstRelaysAlg;
protected:
    bool free_ = true;
    std::vector<Node*> long_or_medium_edge_adj_nodes_;
    std::vector<Node*> short_edge_adj_nodes_;
    std::vector<Node*> type_1_relays_;

    virtual std::string getStringContent() const override;

public:
    MSTNodeLam(MSTNode* node, bool deep_copy=true);
    virtual std::string getNodeTypeStr() const override;
    virtual ~MSTNodeLam();
};

#endif //DCTC_ALGORITHMS_CONNECTIVITY_LAM_MST_NODE_LAM_H_