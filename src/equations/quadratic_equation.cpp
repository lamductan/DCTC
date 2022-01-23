#include <cmath>
#include <utility>

#include "equations/quadratic_equation.h"


QuadraticEquation::QuadraticEquation(long double a, long double b, long double c) : a(a), b(b), c(c) {
    delta = b*b - a*c*4;
}

int QuadraticEquation::getNumberOfSolutions() const {
    if (delta < 0) return 0;
    if (fabsl(delta) < 1e-9) return 1;
    return 2;
}

std::pair<long double, long double> QuadraticEquation::solve() const {
    if (delta < 0) return {INFINITY, INFINITY};
    long double sqrtl_delta = sqrtl(delta);
    return {(-b - sqrtl_delta)/(a*2), (-b + sqrtl_delta)/(a*2)};
}