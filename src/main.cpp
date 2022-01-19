#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "dctc/utils.h"
#include "geometric_primitives/geom2D.h"
#include "dctc/instance/instance.h"


void test1() {
    Point2D A(0, 0);
    Point2D B(5, 0);
    Point2D C(4, 3);
    Point2D D(2, 4);
    Point2D E(-1, 4);
    Point2D F(-2, -1);
    Point2D G(-4, 0);
    Point2D H(0, -4);
    Point2D I(2, -3);
    Point2D J(4, -2);
    Point2D K(7, -1);

    std::cout << "BAX" << '\n';
    std::vector<Point2D> points1{C, D, E, F, G, H, I, J, K};
    std::vector<char> point_names1{'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    for(int i = 0; i < points1.size(); ++i) {
        double angle = computeAngle(B, A, points1[i]);
        double geometric_angle = computeGeometricAngle(B, A, points1[i]);
        std::cout << "angle BA" << point_names1[i] << " = " 
            << rad2deg(angle) << ' ' << rad2deg(geometric_angle) << ' ' << computeCCW(B, A, points1[i]) << '\n';
    }

    std::cout << "=========================" << '\n';
    std::cout << "GAX" << '\n';
    std::vector<Point2D> points2{E, D, C, B, K, J, I, H, F};
    std::vector<char> point_names2{'E', 'D', 'C', 'B', 'K', 'J', 'I', 'H', 'F'};
    for(int i = 0; i < points2.size(); ++i) {
        double angle = computeAngle(G, A, points2[i]);
        double geometric_angle = computeGeometricAngle(G, A, points2[i]);
        std::cout << "angle GA" << point_names2[i] << " = "
            << rad2deg(angle) << ' ' << rad2deg(geometric_angle) << ' ' << computeCCW(G, A, points2[i]) << '\n';
    }
}

void test2() {
    std::vector<Point2D*> p;
    Vector2D v1(1, 1);
    Vector2D v2(0, 0);
    p.push_back(&v1);
    p.push_back(&v2);
    for(auto x : p) std::cout << *x << ' ';
    std::cout << '\n';
}

void test3() {
    Sector sector(Point2D(1, 1), 5, PI_2, PI_3);
    std::cout << sector << '\n';
}

void test4() {
    bool deterministic = true;
    int n_targets = 5;
    double min_range = 0;
    double max_range = 10;
    NodeType node_type = SENSING_DD_NODE;
    double r_s = 2.0;
    double r_c = 1.0;
    double theta_s = PI_3;
    double theta_c = PI_2;

    Instance instance(
        n_targets, min_range, max_range,
        node_type, r_s, r_c, theta_s, theta_c,
        deterministic);
    std::vector<Node*> coverage_sensors = instance.putCoverageSensors(TRIVIAL_COVERAGE_ALG);
    MSTGraph* MST_graph_ptr = Instance::constructMSTGraphCoverageSensors(coverage_sensors);
    std::cout << *MST_graph_ptr << '\n';
    delete MST_graph_ptr;
}

int main() {
    test4();
    return 0;
}
