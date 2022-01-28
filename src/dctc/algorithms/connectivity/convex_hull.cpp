#include <cassert>
#include <algorithm>
#include <stack>
#include <vector>

#include "geometric_primitives/geom2D.h"
#include "dctc/algorithms/connectivity/utils.h"


// Reference: https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/

// A utility function to find next to top in a stack
Node* nextToTop(std::stack<Node*> &S) {
    Node* p = S.top();
    S.pop();
    Node* res = S.top();
    S.push(p);
    return res;
}

int orientation_node(Node* np, Node* nq, Node* nr) {
    return orientation(np->getPoint2D(), nq->getPoint2D(), nr->getPoint2D());
}

std::vector<Node*> findConvexHull(std::vector<Node*> nodes) {
    int n = nodes.size();
    std::vector<Node*> convex_hull;
    if (n == 0) return convex_hull;

    // Find the bottommost point
    long double ymin = nodes[0]->getY();
    int min = 0;
    for (int i = 1; i < n; i++) {
        long double y = nodes[i]->getY();
        if ((y < ymin) || (approx(y, ymin) && nodes[i]->getX() < nodes[min]->getX())) {
            ymin = nodes[i]->getY(), min = i;
        }
    }
 
    // Place the bottom-most point at first position
    std::swap(nodes[0], nodes[min]);
    Node* n0 = nodes[0];
    sort(nodes.begin() + 1, nodes.end(),
         [&](Node* n1, const Node* n2) {
            Point2D p0 = n0->getPoint2D();
            Point2D p1 = n1->getPoint2D();
            Point2D p2 = n2->getPoint2D();
            int o = orientation(p0, p1, p2);
            if (o == 0)
                return (computeEuclidDistance(p0, p1) <= computeEuclidDistance(p0, p2));
            return (o == 2);
         }
    );
    
    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n-1 && orientation_node(n0, nodes[i], nodes[i + 1]) == 0)
            i++;
        nodes[m] = nodes[i];
        m++;
    }
 
    if (m < 3) return convex_hull;
    
    std::stack<Node*> S;
    S.push(nodes[0]);
    S.push(nodes[1]);
    S.push(nodes[2]);
    
    // Process remaining n-3 points
    for (int i = 3; i < m; i++) {
        while (S.size() > 1 && orientation_node(nextToTop(S), S.top(), nodes[i]) != 2)
            S.pop();
        S.push(nodes[i]);
    }
    
    // Now stack has the output points, print contents of stack
    while (!S.empty()) {
        convex_hull.push_back(S.top());
        S.pop();
    }
    std::reverse(convex_hull.begin(), convex_hull.end());
    return convex_hull;
}