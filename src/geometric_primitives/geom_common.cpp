#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "geometric_primitives/geom_common.h"

bool approx(long double x, long double y) {
    return fabsl(x - y) <= EPSILON;
}

int findLargestAbsoluteValueCoefPos(const std::vector<long double>& vDouble) {
    assert(vDouble.size() > 0);
    int largest_absolute_value_coef_pos = 0;
    for(int i = 1; i < vDouble.size(); ++i) {
        if (fabsl(vDouble[i]) > fabsl(vDouble[largest_absolute_value_coef_pos])) {
            largest_absolute_value_coef_pos = i;
        }
    }
    return largest_absolute_value_coef_pos;
}

long double rad2deg(long double rad) {
    return rad*180.0/PI;
}

long double deg2rad(long double deg) {
    return deg/180.0*PI;
}

long double computeGeometricAngle(long double angle) {
    return (angle <= PI) ? angle : TWO_PI - angle;
}

long double standardize_angle(long double angle) {
    if (angle >= 0 && angle < TWO_PI) return angle;
    int k = fabsl(angle)/TWO_PI;
    if (angle < 0) angle += k*TWO_PI;
    else angle -= k*TWO_PI;
    if (angle < 0) return angle + TWO_PI;
    if (angle >= TWO_PI) return angle - TWO_PI;
    return angle;
}

long double round_double(long double var) {
    long double value = (int)(var * 100 + .5);
    return (long double)value / 100;
}

bool almost_eq(long double x, long double y, long double allowed_diff) {
    return fabsl(x - y) <= allowed_diff;
}