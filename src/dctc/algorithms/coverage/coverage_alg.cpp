#include <cassert>
#include <algorithm>
#include <set>

#include "geometric_primitives/geom2D.h"
#include "dctc/algorithms/coverage/coverage_alg.h"
#include "dctc/network_components/nodes/DD_node.h"


std::vector<Node*> trivialCoverageAlg(const Instance* instance) {
    NodeType node_type = instance->getNodeType();
    long double r_s = instance->getRS();
    long double r_c = instance->getRC();
    long double theta_s = instance->getThetaS();
    long double theta_c = instance->getThetaC();
    const std::vector<Point2D>& targets = instance->getTargets();

    std::vector<Node*> nodes;
    for(const Point2D& target : targets) {
        Node* node = new DDNode(target.getX(), target.getY(), node_type, r_s, r_c, theta_s, theta_c);
        nodes.push_back(node);
    }
    return nodes;
}

bool checkCoverageOneTarget(const Point2D& target, const std::vector<Node*>& nodes) {
    for(Node* node : nodes) {
        if (node->getSensingSector()->containsPoint2D(target)) {
            return true;
        }
    }
    return false;
}

bool checkCoverage(const std::vector<Point2D>& targets, const std::vector<Node*>& nodes) {
    for(const Point2D& target : targets) {
        if (!checkCoverageOneTarget(target, nodes)) return false;
    }
    return true;
}

std::vector<Node*> stripCoverageAlg(const Instance* instance) {
    NodeType node_type = instance->getNodeType();
    long double r_s = instance->getRS();
    long double r_c = instance->getRC();
    long double theta_s = instance->getThetaS();
    long double theta_c = instance->getThetaC();
    const std::vector<Point2D>& targets = instance->getTargets();

    int n = targets.size();
    std::vector<Node*> nodes;
    long double ymin = INT_MAX;
    long double ymax = INT_MIN;
    std::vector<std::pair<Point2D, int>> points;
    for(int i = 0; i < n; ++i) {
        points.push_back({targets[i], i});
        long double y = targets[i].getY();
        if (y < ymin) ymin = y;
        else if (y > ymax) ymax = y;
    }

    long double half_theta_s = theta_s/2;
    long double h = r_s*sinl(half_theta_s);
    long double w = r_s/2*(sqrtl(4 - powl(sinl(half_theta_s), 2)) - cosl(half_theta_s));
    int n_strips = ceil((ymax + EPSILON - ymin)/h) + 1;
    long double distance_rectangle_to_center = r_s/2*cos(half_theta_s);
    long double half_h = h/2;

    std::vector<std::vector<std::pair<Point2D, int>>> strips(n_strips);
    std::vector<long double> y_strips;
    for(int i = 0; i < n_strips; ++i) y_strips.push_back(ymin + h*i);
    for(int i = 0; i < n; ++i) {
        long double y = points[i].first.getY();
        int strip_id = std::upper_bound(y_strips.begin(), y_strips.end(), y) - y_strips.begin() - 1;
        strips[strip_id].push_back(points[i]);
    }

    std::set<Point2D> used_pos;
    for(int i = 0; i < n_strips; ++i) {
        int m = strips[i].size();
        if (m == 0) continue;
        long double y_sector = y_strips[i] + half_h;
        std::sort(strips[i].begin(), strips[i].end());
        int j = 0;
        long double x_cur = strips[i][j].first.getX();
        long double x_next = x_cur + w;
        while (j < m) {
            std::pair<Point2D, int> cur_target = strips[i][j];
            long double x = cur_target.first.getX();
            int id = cur_target.second;
            if (x >= x_next) {
                Point2D new_pos(x_cur - distance_rectangle_to_center, y_sector);
                assert(used_pos.count(new_pos) == 0);
                Node* node = new DDNode(
                    new_pos.getX(), new_pos.getY(), node_type, r_s, r_c, theta_s, theta_c);
                used_pos.insert(new_pos);
                nodes.push_back(node);
                x_cur = x;
                x_next = x_cur + w;
            }
            ++j;
        }
        Point2D new_pos(x_cur - distance_rectangle_to_center, y_sector);
        assert(used_pos.count(new_pos) == 0);
        Node* node = new DDNode(
            new_pos.getX(), new_pos.getY(), node_type, r_s, r_c, theta_s, theta_c);
        used_pos.insert(new_pos);
        nodes.push_back(node);
    }

    for(const Point2D& target : targets) {
        if (!checkCoverageOneTarget(target, nodes)) {
            std::cout << "Can't cover target " << target << '\n';
        }
    }

    assert(checkCoverage(targets, nodes));
    std::cout << "Strip coverage alg: OK. There are " << n << " targets and " << nodes.size() << " sensors.\n";
    return nodes;
}