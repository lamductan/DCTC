#include <iostream>
#include <fstream>

#include "graph.h"
#include "draw_utils.h"

/* Node */
Node::Node(long double x, long double y, long double r, long double angle, int node_type)
: x_(x), y_(y), r_(r), angle_(angle), node_type_(node_type) {}

void Node::draw(CImg<unsigned char>& img) {
    x = x_*SCALE;
    y = y_*SCALE;
    r = r_*SCALE;
    const unsigned char* color;
    unsigned int node_size = RELAY_NODE_SIZE;
    if (node_type_ == 4) {
        color = draw_utils::RED;
        node_size = SENSING_NODE_SIZE;
    }
    else if (node_type_ == 8) color = draw_utils::GREEN; // type1 relay
    else if (node_type_ == 9) color = draw_utils::YELLOW; // type2 relay
    else if (node_type_ == 10) color = draw_utils::BLUE; // type3 (short edge) relay
    else if (node_type_ == 11) color = draw_utils::CYAN; // type4 relay

    img.draw_circle(x, y, node_size, color);
    
    if (node_type_ == 4) {
        draw_utils::draw_sector_1(img, x, y, r, angle_, 0, color, FILL_OPACITY);
    }
}

/* Graph */
Graph::Graph() {}

void Graph::set_targets(const std::vector<std::pair<long double, long double>>& targets) {targets_ = targets;}

void Graph::draw_targets(const std::vector<std::pair<long double, long double>>& targets) {
    for(const std::pair<long double, long double>& target : targets) {
        img_.draw_circle((target.first - diff_w)*SCALE, (target.second - diff_h)*SCALE, POINT_SIZE, draw_utils::BLACK);
    }
}

void Graph::draw(const std::string& save_img_path) {
    for(std::pair<Point, Point>& edge : edges_) {
        draw_utils::draw_line(
            img_,
            edge.first.x_*SCALE, edge.first.y_*SCALE,
            edge.second.x_*SCALE, edge.second.y_*SCALE,
            draw_utils::BLACK, LINE_WIDTH);
    }
    
    for(Node& node : nodes_) {
        node.draw(img_);
    }

    img_.save(save_img_path.c_str());
}

Graph Graph::load(const std::string& path) {
    std::ifstream fin;
    fin.open(path);
    Graph graph;

    long double min_x = INT_MAX;
    long double max_x = 0;
    long double min_y = INT_MAX;
    long double max_y = 0;

    fin >> graph.n_nodes_;
    long double r;
    for(int i = 0; i < graph.n_nodes_; ++i) {
        long double x, y, angle;
        int node_type;
        fin >> x >> y >> r >> angle >> node_type;
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
        Node node(x, y, r, angle, node_type);
        graph.nodes_.push_back(node);
    }

    graph.w = max_x - min_x + r*3;
    graph.h = max_y - min_y + r*3;

    long double diff_w = min_x - r*1.5;
    long double diff_h = min_y - r*1.5;
    graph.diff_w = diff_w;
    graph.diff_h = diff_h;

    for(int i = 0; i < graph.n_nodes_; ++i) {
        graph.nodes_[i].x_ -= diff_w;
        graph.nodes_[i].y_ -= diff_h;
    }

    fin >> graph.m_edges_;
    for(int i = 0; i < graph.m_edges_; ++i) {
        Point p1, p2;
        fin >> p1.x_ >> p1.y_;
        fin >> p2.x_ >> p2.y_;
        p1.x_ -= diff_w; p1.y_ -= diff_h;
        p2.x_ -= diff_w; p2.y_ -= diff_h;
        graph.edges_.push_back({p1, p2});
    }

    fin.close();
    graph.img_ = CImg<unsigned char>(graph.w*100, graph.h*100, 1, 3);
    graph.img_.fill(255);
    return graph;
}
