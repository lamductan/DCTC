#include <set>
#include <iostream>
#include <fstream>

#include "instance.h"

Instance::Instance() {}

void Instance::init(
    int n_targets, long double min_range, long double max_range,
    long double r_s, long double r_c, long double theta_s, long double theta_c,
    bool deterministic, long unsigned int seed, bool regen_targets)
{
    deterministic_ = deterministic;
    seed_ = seed;
    regen_targets_ = regen_targets;

    n_targets_ = n_targets;
    min_range_ = min_range;
    max_range_ = max_range;
    r_s_ = r_s;
    r_c_ = r_c;
    theta_s_ = theta_s;
    theta_c_ = theta_c;
    if (regen_targets_) gen_targets();
}

Instance::Instance(
    int n_targets, long double min_range, long double max_range,
    long double r_s, long double r_c, long double theta_s, long double theta_c,
    bool deterministic, long unsigned int seed, bool regen_targets)
{
    init(
        n_targets, min_range, max_range,
        r_s, r_c, theta_s, theta_c,
        deterministic, seed, regen_targets);
}

Instance::Instance(const Instance& other) {
    deterministic_ = other.deterministic_;
    seed_ = other.seed_;
    regen_targets_ = other.regen_targets_;

    n_targets_ = other.n_targets_;
    min_range_ = other.min_range_;
    max_range_ = other.max_range_;
    r_s_ = other.r_s_;
    r_c_ = other.r_c_;
    theta_s_ = other.theta_s_;
    theta_c_ = other.theta_c_;
    targets_ = other.targets_;
}

long double Instance::getMinRange() const {return min_range_;}

long double Instance::getMaxRange() const {return max_range_;}

bool Instance::getDeterministic() const {return deterministic_;}

long double Instance::getRS() const {return r_s_;}

long double Instance::getRC() const {return r_c_;}

long double Instance::getThetaS() const {return theta_s_;}

long double Instance::getThetaC() const {return theta_c_;}

std::vector<std::pair<long double, long double>> Instance::getTargets() const {return targets_;}

void Instance::gen_targets() {}

Instance Instance::load(const std::string& path) {
    std::ifstream fin;
    fin.open(path);

    Instance instance;
    int node_type;
    fin >> instance.n_targets_ >> instance.min_range_ >> instance.max_range_ >> node_type;
    fin >> instance.r_s_ >> instance.r_c_ >> instance.theta_s_ >> instance.theta_c_;
    for(int i = 0; i < instance.n_targets_; ++i) {
        long double x, y;
        fin >> x >> y;
        instance.targets_.push_back({x, y});
    }
    fin.close();
    return instance;
}