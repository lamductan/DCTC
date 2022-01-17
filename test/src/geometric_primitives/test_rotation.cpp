#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/point2D.h"
#include "geometric_primitives/rotation.h"
#include "geometric_primitives/geom_utils_1.h"


TEST(TestRotation, TestRotation1)
{
    Point2D A(1.0, sqrt(2));
    Point2D O(sqrt(5), sqrt(0.5));
    double angle = 0.235;
    Point2D R = rotate(A, O, angle);
    double angle1 = computeAngle(A, O, R);
    ASSERT_DOUBLE_EQ(angle, angle1); 
}

TEST(TestRotation, TestRotation2)
{
    Point2D A(1.0, sqrt(2));
    Point2D O(sqrt(5), sqrt(0.5));
    double angle = TWO_PI;
    Point2D R = rotate(A, O, angle);
    double angle1 = computeAngle(A, O, R);
    ASSERT_DOUBLE_EQ(standardize_angle(angle), angle1); 
}