#include "random/generator.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/coverage/coverage_alg.h"
#include "data_structures/counter.h"


Instance::Instance(
    int n_targets, double min_range, double max_range,
    NodeType node_type, double r_s, double r_c, double theta_s, double theta_c,
    bool deterministic)
{
    n_targets_ = n_targets;
    min_range_ = min_range;
    max_range_ = max_range;
    node_type_ = node_type;
    deterministic_ = deterministic;
    r_s_ = r_s;
    r_c_ = r_c;
    theta_s_ = theta_s;
    theta_c_ = theta_c;
    Counter::resetCount();
    gen_targets();
}

double Instance::getMinRange() const {return min_range_;}

double Instance::getMaxRange() const {return max_range_;}

bool Instance::getDeterministic() const {return deterministic_;}

NodeType Instance::getNodeType() const {return node_type_;}

double Instance::getRS() const {return r_s_;}

double Instance::getRC() const {return r_c_;}

double Instance::getThetaS() const {return theta_s_;}

double Instance::getThetaC() const {return theta_c_;}

std::vector<Point2D> Instance::getTargets() const {return targets_;}

void Instance::gen_targets() {
    DoubleGenerator generator(min_range_, max_range_, deterministic_); 
    for(int i = 0; i < n_targets_; ++i) {
        Point2D target(round(generator.next()), round(generator.next()));
        targets_.push_back(target);
    }
}

std::vector<Node*> Instance::putCoverageSensors(CoverageAlgType coverageAlgType) const {
    if (coverageAlgType == TRIVIAL_COVERAGE_ALG)
        return trivialCoverageAlg(this);
    //else if (coverageAlgType == GREEDY_COVERAGE_ALG)
    //    return greedyCoverageAlg(this);
    //else if (coverageAlgType == STRIP_COVERAGE_ALG)
    //    return stripCoverageAlg(this);
    return trivialCoverageAlg(this);
}

const MSTGraph& Instance::constructMSTGraphCoverageSensors(const std::vector<Node*>& coverage_sensors) {
    return *(new MSTGraph(coverage_sensors));
}