#ifndef DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_

#include <string>
#include "dctc/network_components/nodes/node.h"
#include "geometric_primitives/geom2D.h"


class DONode: public Node {
private:
    Sector sensing_sector_;
    Circle communication_antenna_;
    void init(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s);
    std::string getStringContent() const override;
public:
    DONode();
    DONode(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s);
    DONode(const DONode& other);
    std::string getNodeTypeStr() const override;
    virtual Node* deepCopy() const override;
};

#endif //DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_