#include <random>

#include "random/generator.h"


void Generator::init(bool deterministic) {
    if (deterministic) seed = 1;
    else seed = static_cast<long unsigned int>(time(NULL));
    eng = std::default_random_engine{seed};
}

DoubleGenerator::DoubleGenerator(double min_range, double max_range, bool deterministic) {
    init(deterministic);
    dis = std::uniform_real_distribution<double>(min_range, max_range);
}

double DoubleGenerator::next() {return dis(eng);}

IntGenerator::IntGenerator(int min_range, int max_range, bool deterministic) {
    init(deterministic);
    dis = std::uniform_int_distribution<int>(min_range, max_range);
}

int IntGenerator::next() {return dis(eng);}