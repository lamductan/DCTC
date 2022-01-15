#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "geometric_primitives/geom_common.h"

bool approx(double x, double y) {
    return fabs(x - y) <= EPSILON;
}

int findLargestAbsoluteValueCoefPos(const std::vector<double>& vDouble) {
    assert(vDouble.size() > 0);
    int largest_absolute_value_coef_pos = 0;
    for(int i = 1; i < vDouble.size(); ++i) {
        if (fabs(vDouble[i]) > fabs(vDouble[largest_absolute_value_coef_pos])) {
            largest_absolute_value_coef_pos = i;
        }
    }
    return largest_absolute_value_coef_pos;
}

double rad2deg(double rad) {
    return rad*180.0/PI;
}

double deg2rad(double deg) {
    return deg/180.0*PI;
}

double computeGeometricAngle(double angle) {
    return (angle <= PI) ? angle : TWO_PI - angle;
}

double standardize_angle(double angle) {
    if (angle >= 0 && angle < TWO_PI) return angle;
    int k = fabs(angle)/TWO_PI;
    if (angle < 0) angle += k*TWO_PI;
    else angle -= k*TWO_PI;
    if (angle < 0) return angle + TWO_PI;
    if (angle >= TWO_PI) return angle - TWO_PI;
    return angle;
}

double round_double(double var) {
    double value = (int)(var * 100 + .5);
    return (double)value / 100;
}

bool almost_eq(double x, double y, double allowed_diff) {
    return fabs(x - y) <= allowed_diff;
}