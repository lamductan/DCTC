#ifndef DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_

#include <vector>
#include <utility>

#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"


class MSTNode : public Node {
protected:
    std::vector<std::pair<Node*, Edge*>> mst_edges_;
    bool is_leaf_ = false;
    std::vector<std::pair<Node*, Edge*>> communication_edges_;

public:
    MSTNode(Node* node);
    MSTNode(const MSTNode& other);
    virtual std::string getNodeTypeStr() const override;
    virtual Node* deepCopy() const override;
    virtual Edge* addCommunicationEdge(Node* node);

    virtual ~MSTNode();
};

#endif //DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_