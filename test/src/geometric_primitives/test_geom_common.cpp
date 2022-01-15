#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"


TEST(TestGeomUtils, TestGeomUtils1)
{
    double x = 0.1;
    double y = 0.1 + 1e-10;
    ASSERT_TRUE(approx(x, y));
}