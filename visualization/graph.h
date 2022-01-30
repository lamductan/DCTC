#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <utility>
#include <string>

#include "CImg.h"
using namespace cimg_library;

const unsigned int LINE_WIDTH = 5;
const unsigned int SCALE = 100;
const unsigned int POINT_SIZE = 10;
const unsigned int SENSING_NODE_SIZE = 30;
const unsigned int RELAY_NODE_SIZE = 15;
const double FILL_OPACITY = 0.2;

struct Point {
    long double x_;
    long double y_;

    Point() {}
    Point(long double x, long double y): x_(x), y_(y) {}
};

class Node {
public:
    int x;
    int y;
    int r;
    long double x_;
    long double y_;
    long double r_;
    long double angle_;
    int node_type_;

    Node(long double x, long double y, long double r, long double angle, int node_type);
    void draw(CImg<unsigned char>& img);
};

class Graph {
public:
    int n_nodes_;
    int m_edges_;
    std::vector<Node> nodes_;
    std::vector<std::pair<Point, Point>> edges_;
    CImg<unsigned char> img_;
    long double w;
    long double h;
    long double diff_w;
    long double diff_h;
    std::vector<std::pair<long double, long double>> targets_;
    
public:
    Graph();
    static Graph load(const std::string& path);
    void set_targets(const std::vector<std::pair<long double, long double>>& targets);
    void draw_targets(const std::vector<std::pair<long double, long double>>& targets);
    void draw(const std::string& save_img_path);
};

#endif //GRAPH_H_
