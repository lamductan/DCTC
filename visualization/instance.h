#ifndef INSTANCE_INSTANCE_H_
#define INSTANCE_INSTANCE_H_

#include <vector>
#include <utility>
#include <string>


class Instance {
private:
    bool deterministic_;
    long unsigned int seed_;
    bool regen_targets_;

    int n_targets_;
    long double min_range_;
    long double max_range_;
    std::vector<std::pair<long double, long double>> targets_;
    long double r_s_;
    long double r_c_;
    long double theta_s_;
    long double theta_c_;

    void init(
        int n_targets, long double min_range, long double max_range, 
        long double r_s, long double r_c, long double theta_s, long double theta_c,
        bool deterministic=false, long unsigned int seed=1, bool regen_targets=true);
    void gen_targets();

public:
    Instance();
    Instance(
        int n_targets, long double min_range, long double max_range, 
        long double r_s, long double r_c, long double theta_s, long double theta_c,
        bool deterministic=false, long unsigned int seed=1, bool regen_targets=true);
    Instance(const Instance& other);
    long double getMinRange() const;
    long double getMaxRange() const;
    bool getDeterministic() const;
    long double getRS() const;
    long double getRC() const;
    long double getThetaS() const;
    long double getThetaC() const;
    std::vector<std::pair<long double, long double>> getTargets() const;

    void save(const std::string& path) const;
    static Instance load(const std::string& path);
};

#endif //INSTANCE_INSTANCE_H_