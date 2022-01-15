#include <iostream>
#include <cmath>
#include <utility>
#include <gtest/gtest.h>

#include "equations/quadratic_equation.h"


TEST(TestQuadraticEquation, TestQuadraticEquation_1)
{
    QuadraticEquation quadratic_equation(1, -2, 1);
    ASSERT_EQ(quadratic_equation.getNumberOfSolutions(), 1);
    std::pair<double, double> solutions = quadratic_equation.solve();
    ASSERT_DOUBLE_EQ(solutions.first, 1.0);
    ASSERT_DOUBLE_EQ(solutions.second, 1.0);
}

TEST(TestQuadraticEquation, TestQuadraticEquation_2)
{
    QuadraticEquation quadratic_equation(1, 2, 2);
    ASSERT_EQ(quadratic_equation.getNumberOfSolutions(), 0);
}

TEST(TestQuadraticEquation, TestQuadraticEquation_3)
{
    QuadraticEquation quadratic_equation(1, 1, -2);
    ASSERT_EQ(quadratic_equation.getNumberOfSolutions(), 2);
    std::pair<double, double> solutions = quadratic_equation.solve();
    ASSERT_DOUBLE_EQ(solutions.first, -2.0);
    ASSERT_DOUBLE_EQ(solutions.second, 1.0);
}
