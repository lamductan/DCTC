#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/nodes/MST_node.h"
#include "dctc/network_components/nodes/node_factory.h"
#include "dctc/algorithms/connectivity/Tran/MST_node_Tran.h"


Node* NodeFactory::createNode(Node* node, GraphNodeType graph_node_type) {
    if (graph_node_type == BASE_GRAPH_NODE) {
        return new Node(*node);
    } else if (graph_node_type == MST_GRAPH_NODE) {
        return new MSTNode((MSTNode*) node);
    } else if (graph_node_type == MST_GRAPH_NODE_TRAN) {
        return new MSTNodeTran((MSTNode*) node);
    }
    return nullptr;
}