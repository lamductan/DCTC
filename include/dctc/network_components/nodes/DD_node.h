#ifndef DCTC_NETWORK_COMPONENTS_NODES_DD_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_DD_NODE_H_

#include <string>

#include "dctc/network_components/nodes/node.h"
#include "geometric_primitives/geom2D.h"


class DDNode: public Node {
private:
    Sector sensing_sector_;
    Sector communication_antenna_;
    void init(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s, double theta_c);
    std::string getStringContent() const override;
public:
    DDNode();
    DDNode(double x, double y, NodeType node_type, double r_s, double r_c, double theta_s, double theta_c);
    DDNode(const DDNode& other);
    std::string getNodeTypeStr() const override;
    virtual Node* deepCopy() const override;

};

#endif //DCTC_NETWORK_COMPONENTS_NODES_DD_NODE_H_