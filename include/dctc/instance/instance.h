#ifndef DCTC_INSTANCE_INSTANCE_H_
#define DCTC_INSTANCE_INSTANCE_H_

#include <vector>

#include "geometric_primitives/geom2D.h"
#include "dctc/network_components/graphs/MST_graph.h"


enum CoverageAlgType {
    TRIVIAL_COVERAGE_ALG,
    GREEDY_COVERAGE_ALG,
    STRIP_COVERAGE_ALG,
};

enum CommunicationAlgType {
    TRIVIAL_COMMUNICATION_ALG,
    LAM_LONG_EDGE_FIRST_COMMUNICATION_ALG,
    LAM_SHORT_EDGE_FIRST_COMMUNICATION_ALG,
    TRAN_COMMUNICATION_ALG,
    ASCHNER_COMMUNICATION_ALG,
};

class Instance {
private:
    int n_targets_;
    double min_range_;
    double max_range_;
    bool deterministic_;
    std::vector<Point2D> targets_;
    NodeType node_type_;
    double r_s_;
    double r_c_;
    double theta_s_;
    double theta_c_;

    void gen_targets();

public:
    Instance(
        int n_targets, double min_range, double max_range, 
        NodeType node_type, double r_s, double r_c, double theta_s, double theta_c,
        bool deterministic=false);
    double getMinRange() const;
    double getMaxRange() const;
    bool getDeterministic() const;
    NodeType getNodeType() const;
    double getRS() const;
    double getRC() const;
    double getThetaS() const;
    double getThetaC() const;
    std::vector<Point2D> getTargets() const;
    std::vector<Node*> putCoverageSensors(CoverageAlgType coverage_alg_type) const;
    static MSTGraph* constructMSTGraphCoverageSensors(const std::vector<Node*>& coverage_sensors);
};

#endif //DCTC_INSTANCE_INSTANCE_H_