#include <iostream>
#include <gtest/gtest.h>

#include "dctc/utils.h"


class TestUtils : public ::testing::Test {
protected:
    std::unordered_map<int, int> m;

    void SetUp() override {
        for(int i = 0; i < 10; ++i) {
            m[i] = -i;
        }
    }
};

TEST_F(TestUtils, TestGetKeys)
{
    std::vector<int> keys = getMapKeys<int, int>(m);
    for(int key : keys) std::cout << key << ' ';
    std::cout << '\n';
}

TEST_F(TestUtils, TestGetValues)
{
    std::vector<int> values = getMapValues<int, int>(m);
    for(int value : values) std::cout << value << ' ';
    std::cout << '\n';
}