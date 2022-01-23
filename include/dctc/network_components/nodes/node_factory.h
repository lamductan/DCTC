#ifndef DCTC_NETWORK_COMPOMENTS_NODES_NODE_FACTORY_H_
#define DCTC_NETWORK_COMPOMENTS_NODES_NODE_FACTORY_H_

#include "dctc/network_components/nodes/node.h"

enum GraphNodeType {
    BASE_GRAPH_NODE,
    MST_GRAPH_NODE,
    MST_GRAPH_NODE_TRAN,
    MST_GRAPH_NODE_ASCHNER,
};

class NodeFactory {
public:
    static Node* createNode(Node* node, GraphNodeType graph_node_type, bool deep_copy=true);
};

#endif //DCTC_NETWORK_COMPOMENTS_NODES_NODE_FACTORY_H_