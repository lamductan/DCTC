#include <algorithm>
#include <stack>
#include <vector>
#include "geometric_primitives/convex_hull.h"
#include "geometric_primitives/geom_utils_1.h"

// Reference: https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/

// A utility function to find next to top in a stack
Point2D nextToTop(std::stack<Point2D> &S) {
    Point2D p = S.top();
    S.pop();
    Point2D res = S.top();
    S.push(p);
    return res;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(const Point2D& p, const Point2D& q, const Point2D& r) {
    long double val = computeCCW(q, p , r);
    if (fabsl(val) <= EPSILON) return 0;  // collinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

std::vector<Point2D> findConvexHull(std::vector<Point2D> points) {
    int n = points.size();
    std::vector<Point2D> convex_hull;
    if (n == 0) return convex_hull;

    // Find the bottommost point
    long double ymin = points[0].getY(), min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].getY();
        if ((y < ymin) || (ymin == y && points[i].getX() < points[min].getX()))
            ymin = points[i].getY(), min = i;
    }
 
    // Place the bottom-most point at first position
    std::swap(points[0], points[min]);
    Point2D p0 = points[0];
    sort(points.begin() + 1, points.end(),
         [&](const Point2D& p1, const Point2D& p2) {
            int o = orientation(p0, p1, p2);
            if (o == 0)
                return (computeEuclidDistance(p0, p1) <= computeEuclidDistance(p0, p2));
            return (o == 2);
         }
    );
    
    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n-1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }
 
    if (m < 3) return convex_hull;
    
    std::stack<Point2D> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);
    
    // Process remaining n-3 points
    for (int i = 3; i < m; i++) {
        while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }
    
    // Now stack has the output points, print contents of stack
    while (!S.empty()) {
        convex_hull.push_back(S.top());
        S.pop();
    }
    std::reverse(convex_hull.begin(), convex_hull.end());
    return convex_hull;
}