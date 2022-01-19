#ifndef DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_
#define DCTC_NETWORK_COMPONENTS_NODES_MST_NODE_H_

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

#include "dctc/network_components/nodes/node.h"
#include "dctc/network_components/edges/edge.h"


class MSTNode : public Node {
protected:
    Node* node_;
    bool is_leaf_ = false;
    std::unordered_map<Node*, Edge*> mst_edges_;
    std::unordered_map<Node*, Edge*> communication_edges_;

    void init(Node* node);
    void init(MSTNode* node);
    virtual std::string getStringContent() const override;

public:
    MSTNode();
    MSTNode(Node* node);
    MSTNode(MSTNode* node);
    virtual std::string getNodeTypeStr() const;
    virtual Circle* getSensingSector() const;
    virtual Circle* getCommunicationAntenna() const;
    bool isLeaf() const;
    void setIsLeaf(bool is_leaf);

    void addMSTEdge(Edge* edge);
    std::unordered_map<Node*, Edge*> getMSTEdgesMap() const;
    std::vector<Node*> getMSTEdgeAdjNodes() const;
    std::vector<Edge*> getMSTEdges() const;

    void addCommunicationEdge(Edge* edge);
    std::unordered_map<Node*, Edge*> getCommunicationEdgesMap() const;
    std::vector<Node*> getCommunicationEdgeAdjNodes() const;
    std::vector<Edge*> getCommunicationEdges() const;

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