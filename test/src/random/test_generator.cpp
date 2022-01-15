#include <ctime>
#include <iostream>
#include <gtest/gtest.h>

#include "random/generator.h"


TEST(TestGenerator, TestDoubleGenerator_1)
{
    DoubleGenerator double_generator_1(0, 1000);
    sleep(1);
    DoubleGenerator double_generator_2(0, 1000);

    for(int i = 0; i < 10; ++i) std::cout << double_generator_1.next() << ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i) std::cout << double_generator_2.next() << ' ';
    std::cout << '\n';
}

TEST(TestGenerator, TestDoubleGenerator_2)
{
    DoubleGenerator double_generator_1(0, 1000, true);
    sleep(1);
    DoubleGenerator double_generator_2(0, 1000, true);

    for(int i = 0; i < 10; ++i) std::cout << double_generator_1.next() << ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i) std::cout << double_generator_2.next() << ' ';
    std::cout << '\n';
}

TEST(TestGenerator, TestIntGenerator_1)
{
    IntGenerator int_generator_1(0, 1000);
    sleep(1);
    IntGenerator int_generator_2(0, 1000);

    for(int i = 0; i < 10; ++i) std::cout << int_generator_1.next() << ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i) std::cout << int_generator_2.next() << ' ';
    std::cout << '\n';
}

TEST(TestGenerator, TestIntGenerator_2)
{
    IntGenerator int_generator_1(0, 1000, true);
    sleep(1);
    IntGenerator int_generator_2(0, 1000, true);

    for(int i = 0; i < 10; ++i) std::cout << int_generator_1.next() << ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i) std::cout << int_generator_2.next() << ' ';
    std::cout << '\n';
}
