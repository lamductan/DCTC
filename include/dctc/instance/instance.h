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
    bool deterministic_;
    long unsigned int seed_;
    bool regen_targets_;

    int n_targets_;
    long double min_range_;
    long double max_range_;
    std::vector<Point2D> targets_;
    NodeType node_type_;
    long double r_s_;
    long double r_c_;
    long double theta_s_;
    long double theta_c_;

    void init(
        int n_targets, long double min_range, long double max_range, 
        NodeType node_type, long double r_s, long double r_c, long double theta_s, long double theta_c,
        bool deterministic=false, long unsigned int seed=1, bool regen_targets=true);
    void gen_targets();

public:
    Instance();
    Instance(
        int n_targets, long double min_range, long double max_range, 
        NodeType node_type, long double r_s, long double r_c, long double theta_s, long double theta_c,
        bool deterministic=false, long unsigned int seed=1, bool regen_targets=true);
    Instance(const Instance& other);
    long double getMinRange() const;
    long double getMaxRange() const;
    bool getDeterministic() const;
    NodeType getNodeType() const;
    long double getRS() const;
    long double getRC() const;
    long double getThetaS() const;
    long double getThetaC() const;
    std::vector<Point2D> getTargets() const;
    void setTargets(const std::vector<Point2D>& targets);
    std::vector<Node*> putCoverageSensors(CoverageAlgType coverage_alg_type) const;
    static MSTGraph* constructMSTGraphCoverageSensors(const std::vector<Node*>& coverage_sensors);
};

#endif //DCTC_INSTANCE_INSTANCE_H_