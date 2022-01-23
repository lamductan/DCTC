#include <iostream>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"


TEST(TestGeomUtils, TestGeomUtils1)
{
    long double x = 0.1;
    long double y = 0.1 + 1e-10;
    ASSERT_TRUE(approx(x, y));
}