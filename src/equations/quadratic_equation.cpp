#include <cmath>
#include <utility>

#include "equations/quadratic_equation.h"


QuadraticEquation::QuadraticEquation(double a, double b, double c) : a(a), b(b), c(c) {
    delta = b*b - a*c*4;
}

int QuadraticEquation::getNumberOfSolutions() const {
    if (delta < 0) return 0;
    if (fabs(delta) < 1e-9) return 1;
    return 2;
}

std::pair<double, double> QuadraticEquation::solve() const {
    if (delta < 0) return {INFINITY, INFINITY};
    double sqrt_delta = sqrt(delta);
    return {(-b - sqrt_delta)/(a*2), (-b + sqrt_delta)/(a*2)};
}