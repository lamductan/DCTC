#ifndef EQUATIONS_QUADRATIC_EQUATION_H_
#define EQUATIONS_QUADRATIC_EQUATION_H_

#include <cmath>
#include <utility>


class QuadraticEquation {
private:
    double a, b, c;
    double delta;

public:
    QuadraticEquation(double a, double b, double c);
    int getNumberOfSolutions() const;
    std::pair<double, double> solve() const;
};

#endif //EQUATIONS_QUADRATIC_EQUATION_H_
