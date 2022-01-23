#include <cassert>

#include "dctc/utils.h"
#include "dctc/algorithms/connectivity/relays/relays_MST_graph.h"
#include "dctc/algorithms/connectivity/communication_checker.h"

RelaysMSTGraph::RelaysMSTGraph(const std::vector<Node*>& nodes, GraphNodeType graph_node_type, long double r_c, long double theta_c,
                               std::vector<Edge*>& communication_edges, int n_total_nodes_omni)
: MSTGraph(nodes, graph_node_type, false)
{
    r_c_ = r_c;
    theta_c_ = theta_c;
    n_total_nodes_omni_ = n_total_nodes_omni;
    beta_ = (1.0*nodes_.size())/(1.0*n_total_nodes_omni_);
    buildMST(communication_edges);

    for(Node* node : nodes_) {
        if (node->getNodeType() <= SENSING_DD_NODE) {
            terminals_.push_back(node);
        } else {
            relays_.push_back(node);
        }
    }
    assert(CommunicationChecker::checkConnectivityAngleAndRange(this));
    std::cout << "Result: OK. Returned Relays MST Graph satisfies connectivity, angle and range requirements.\n";
    //std::cout << "beta = " << beta_ << '\n';
}

GraphNodeType RelaysMSTGraph::getGraphNodeType() const {return graph_node_type_;}

long double RelaysMSTGraph::getThetaC() const {return theta_c_;}

long double RelaysMSTGraph::getRC() const {return r_c_;}

int RelaysMSTGraph::getNumTerminals() const {return terminals_.size();}

int RelaysMSTGraph::getNumRelays() const {return relays_.size();}

int RelaysMSTGraph::getNumTotalNodes() const {return nodes_.size();}

int RelaysMSTGraph::getNumTotalNodesOmni() const {return n_total_nodes_omni_;}

std::vector<Node*> RelaysMSTGraph::getTerminals() const {return terminals_;}

std::vector<Node*> RelaysMSTGraph::getRelayNodes() const {return relays_;}

std::vector<Node*> RelaysMSTGraph::getAllNodes() const {return nodes_;}

long double RelaysMSTGraph::getBeta() const {return beta_;}
