#include <iostream>
#include <random>

#include "random/generator.h"


void Generator::init(bool deterministic, long unsigned int seed) {
    if (!deterministic && seed == 1) {
        seed = static_cast<long unsigned int>(time(NULL));
    }
    std::cout << "seed = " << seed << '\n';
    eng = std::default_random_engine{seed};
}

long unsigned int Generator::getSeed() const {return seed;}

DoubleGenerator::DoubleGenerator(long double min_range, long double max_range, bool deterministic, long unsigned int seed) {
    init(deterministic, seed);
    dis = std::uniform_real_distribution<long double>(min_range, max_range);
}

long double DoubleGenerator::next() {return dis(eng);}

IntGenerator::IntGenerator(int min_range, int max_range, bool deterministic, long unsigned int seed) {
    init(deterministic, seed);
    dis = std::uniform_int_distribution<int>(min_range, max_range);
}

int IntGenerator::next() {return dis(eng);}