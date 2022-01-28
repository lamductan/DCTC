#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "utils.h"
#include "dctc/instance/instance.h"
#include "dctc/algorithms/coverage/coverage_alg.h"


class TestCoverageAlg : public ::testing::Test {
protected:
    bool deterministic = true;
    long unsigned int seed = 1;

    int n_targets = 20;
    long double min_range = 0;
    long double max_range = 100;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 2.0;
    long double r_c = 1.0;
    long double theta_s = PI_3;
    long double theta_c = PI_2;
    Instance instance;
    std::string instance_save_path = PROJECT_ROOT_PATH + "/data/coverage_instance.dat";

    void SetUp() override {
        if (!deterministic) {
            instance = Instance(
                n_targets, min_range, max_range,
                node_type, r_s, r_c, theta_s, theta_c,
                deterministic, seed);
            instance.save(instance_save_path);
        } else {
            instance = Instance::load(instance_save_path);
        }
    }

    void freeMemory(std::vector<Node*>& coverage_sensors) {
        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
    }
};

TEST_F(TestCoverageAlg, TestStripCoverageAlg)
{
    std::vector<Node*> coverage_sensors = instance.putCoverageSensors(STRIP_COVERAGE_ALG);
    freeMemory(coverage_sensors);
}

TEST(TestCoverageAlg1, TestStripRandomNondeterministic)
{
    int n_tests = 100;
    bool deterministic = false;
    long unsigned int seed = time(NULL);

    int n_targets = 1500;
    long double min_range = 0;
    long double max_range = 10000;
    NodeType node_type = SENSING_DD_NODE;
    long double r_s = 500;
    long double r_c = 500;
    long double theta_s = PI_2;
    long double theta_c = PI_2;
    Instance instance;
    std::vector<Node*> coverage_sensors; 
    std::string instance_save_path = PROJECT_ROOT_PATH + "/data/coverage_instance.dat";

    for(int i = 0; i < n_tests; ++i) {
        std::cout << "\nTest " << i << '\n';
        instance = Instance(
            n_targets, min_range, max_range,
            node_type, r_s, r_c, theta_s, theta_c,
            deterministic, seed + i);
        instance.save(instance_save_path);

        std::vector<Node*> coverage_sensors = instance.putCoverageSensors(STRIP_COVERAGE_ALG);

        for(Node* coverage_sensor : coverage_sensors)
            delete coverage_sensor;
    }
}