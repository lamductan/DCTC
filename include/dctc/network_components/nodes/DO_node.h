#ifndef DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_

#include <string>
#include "dctc/network_components/nodes/node.h"
#include "geometric_primitives/geom2D.h"


class DONode : public Node {
private:
    Sector sensing_sector_;
    Circle communication_antenna_;
    void init(long double x, long double y, NodeType node_type, long double r_s, long double r_c, long double theta_s);
    virtual std::string getStringContent() const override;
public:
    DONode();
    DONode(long double x, long double y, NodeType node_type, long double r_s, long double r_c, long double theta_s);
    DONode(const Point2D& point2D, NodeType node_type, long double r_s, long double r_c, long double theta_s);
    DONode(const DONode& other);
    virtual std::string getNodeTypeStr() const override;
    virtual Node* deepCopy(bool preserve_id=false) const override;
    virtual Circle* getSensingSector() const override;
    virtual Circle* getCommunicationAntenna() const override;
};

#endif //DCTC_NETWORK_COMPONENTS_NODES_DO_NODE_H_