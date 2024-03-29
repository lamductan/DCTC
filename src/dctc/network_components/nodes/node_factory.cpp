#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/nodes/MST_node.h"
#include "dctc/network_components/nodes/node_factory.h"

#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"
#include "dctc/algorithms/connectivity/Lam/MST_node_Lam.h"


Node* NodeFactory::createNode(Node* node, GraphNodeType graph_node_type, bool deep_copy) {
    if (graph_node_type == BASE_GRAPH_NODE) {
        return new Node(*node);
    } else if (graph_node_type == MST_GRAPH_NODE) {
        return new MSTNode((MSTNode*) node, deep_copy);
    } else if (graph_node_type == MST_GRAPH_NODE_TRAN) {
        return new MSTNodeTran((MSTNode*) node);
    } else if (graph_node_type == MST_GRAPH_NODE_LAM) {
        return new MSTNodeLam((MSTNode*) node, deep_copy);
    }
    return nullptr;
}