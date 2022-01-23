#ifndef DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"


class MSTGraph;

class MSTNode : public Node {
friend class MSTGraph;
protected:
    Node* node_;
    bool is_leaf_ = false;
    std::unordered_map<MSTNode*, Edge*> mst_edges_;
    std::unordered_map<Node*, Edge*> communication_edges_;

    int level_in_rooted_mst_;
    std::unordered_map<MSTNode*, Edge*> MST_children_edges_map_;
    std::vector<MSTNode*> MST_children_nodes_;
    std::vector<Edge*> MST_children_edges_;

    MSTNode* parent_in_rooted_MST_ = nullptr;
    Edge* edge_to_parent_in_rooted_MST_;

    long double required_theta_c_ = TWO_PI;

    void init(Node* node, bool deep_copy=true);
    void init(MSTNode* node, bool deep_copy=true);
    virtual std::string getStringContent() const override;

public:
    MSTNode();
    MSTNode(const Point2D& point2D, NodeType node_type);
    MSTNode(Node* node, bool deep_copy=true);
    MSTNode(MSTNode* node, bool deep_copy=true);
    virtual std::string getNodeTypeStr() const override;
    virtual Circle* getSensingSector() const;
    virtual Circle* getCommunicationAntenna() const;
    bool isLeaf() const;
    void setIsLeaf(bool is_leaf);

    void addMSTEdge(Edge* edge);
    std::unordered_map<MSTNode*, Edge*> getMSTEdgesMap() const;
    std::vector<MSTNode*> getMSTEdgeAdjNodes() const;
    std::vector<Edge*> getMSTEdges() const;

    std::vector<MSTNode*> getMSTChildrenNodes() const;
    std::vector<Edge*> getMSTChildrenEdges() const;
    std::unordered_map<MSTNode*, Edge*> getMSTChildrenEdgesMap() const;

    void addCommunicationEdge(Edge* edge);
    std::unordered_map<Node*, Edge*> getCommunicationEdgesMap() const;
    std::vector<Node*> getCommunicationEdgeAdjNodes() const;
    std::vector<Edge*> getCommunicationEdges() const;

    void setRequiredThetaC(long double required_theta_c);

    /** 
     * This method can only be used if users are well aware of the following information.
     * deepCopy doesn't copy edges, because all nodes's pointers are changed,
     * so a node must know all other nodes's pointers to create correct edges.
     * Therefore edges must be added in the Graph's deepCopy function.
     */
    virtual Node* deepCopy(bool preserve_id=false) const;

    virtual ~MSTNode();
    friend std::ostream& operator<<(std::ostream& os, const MSTNode& node);
};

#endif //DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_