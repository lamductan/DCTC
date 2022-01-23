#ifndef RANDOM_GENERATOR_H_
#define RANDOM_GENERATOR_H_

#include <random>


class Generator {
protected:
    long unsigned int seed;
    std::default_random_engine eng;
    void init(bool deterministic=false, long unsigned int seed=1);

public:
    long unsigned int getSeed() const;
};

class DoubleGenerator: public Generator {
private:
    std::uniform_real_distribution<long double> dis;
public:
    DoubleGenerator(long double min_range, long double max_range, bool deterministic=false, long unsigned int seed=1);
    long double next();
};

class IntGenerator: public Generator {
private:
    std::uniform_int_distribution<int> dis;
public:
    IntGenerator(int min_range, int max_range, bool deterministic=false, long unsigned int seed=1);
    int next();
};

#endif //RANDOM_GENERATOR_H_