#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "geometric_primitives/geom_common.h"
#include "geometric_primitives/vector2D.h"


TEST(TestVector2D, TestVector2D_1)
{
    Vector2D v = Vector2D(3.0, 4.0);
    ASSERT_TRUE(approx(v.computeNorm(), 5.0));
}

TEST(TestVector2D, TestVector2D_2)
{
    std::vector<long double> v{3.0, 4.0};
    Vector2D vector2D = VectorToVector2D(v);
    std::cout << vector2D.toString() << std::endl;
    ASSERT_TRUE(true);
}

TEST(TestVector2D, TestVector2D_sameDirection)
{
    Vector2D v1(1, 2);
    Vector2D v2(2, 4);
    ASSERT_TRUE(v1.hasSameDirectionWithVector2D(v2));
    ASSERT_TRUE(v2.hasSameDirectionWithVector2D(v1));

    Vector2D v3(0, 1);
    Vector2D v4(0, 1000);
    ASSERT_TRUE(v3.hasSameDirectionWithVector2D(v4));
    ASSERT_TRUE(v4.hasSameDirectionWithVector2D(v3));

    Vector2D v5(1, 2);
    Vector2D v6(-1, -2);
    ASSERT_FALSE(v5.hasSameDirectionWithVector2D(v6));
    ASSERT_FALSE(v6.hasSameDirectionWithVector2D(v5));
}

TEST(TestVector2D, TestVector2D_oppositeDirection)
{
    Vector2D v1(1, 2);
    Vector2D v2(2, 4);
    ASSERT_FALSE(v1.hasOppositeDirectionWithVector2D(v2));
    ASSERT_FALSE(v2.hasOppositeDirectionWithVector2D(v1));

    Vector2D v3(0, 1);
    Vector2D v4(0, -1000);
    ASSERT_TRUE(v3.hasOppositeDirectionWithVector2D(v4));
    ASSERT_TRUE(v4.hasOppositeDirectionWithVector2D(v3));

    Vector2D v5(1, 2);
    Vector2D v6(-1, -2);
    ASSERT_TRUE(v5.hasOppositeDirectionWithVector2D(v6));
    ASSERT_TRUE(v6.hasOppositeDirectionWithVector2D(v5));
}

TEST(TestVector2D, TestVector2D_print)
{
    Vector2D v1 = Vector2D(3.0, 4.0);
    Vector2D v2 = Vector2D(4.0, 3.0);
    std::vector<Vector2D> v{v1, v2};
    print_vector<Vector2D>(v);
    std::cout << '\n';
    ASSERT_TRUE(true);
}
