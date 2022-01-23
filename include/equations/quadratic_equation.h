#ifndef EQUATIONS_QUADRATIC_EQUATION_H_
#define EQUATIONS_QUADRATIC_EQUATION_H_

#include <cmath>
#include <utility>


class QuadraticEquation {
private:
    long double a, b, c;
    long double delta;

public:
    QuadraticEquation(long double a, long double b, long double c);
    int getNumberOfSolutions() const;
    std::pair<long double, long double> solve() const;
};

#endif //EQUATIONS_QUADRATIC_EQUATION_H_
