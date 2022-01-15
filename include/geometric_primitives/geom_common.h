#ifndef GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_
#define GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_

#include <cmath>
#include <iostream>
#include <vector>
#include <set>


const double EPSILON = 1e-9;
const int DECIMAL_DIGITS = 2;
const double PI = acos(-1);
const double TWO_PI = PI*2;
const double PI_2 = acos(0);
const double PI_4 = PI_2/2;
const double PI_3 = acos(1.0/2.0);
const double PI_2_3 = acos(-1.0/2.0);

bool approx(double x, double y); 
int findLargestAbsoluteValueCoefPos(const std::vector<double>& vDouble);
double rad2deg(double rad);
double deg2rad(double deg);
double computeGeometricAngle(double angle);
double standardize_angle(double angle);

template<typename T>
void print_vector(const std::vector<T>& v, char last_char=0, std::ostream& os=std::cout) {
    os << "[";
    for(const T& t : v) os << t << ", ";
    os << "]";
    if (last_char) os << last_char;
}

template<typename T>
void print_set(const std::set<T>& v, char last_char=0, std::ostream& os=std::cout) {
    os << "[";
    for(const T& t : v) os << t << ", ";
    os << "]";
    if (last_char) os << last_char;
}

template <typename T>
std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B) {
    std::vector<T> AB;
    AB.reserve(A.size() + B.size());                // preallocate memory
    AB.insert(AB.end(), A.begin(), A.end());        // add A;
    AB.insert(AB.end(), B.begin(), B.end());        // add B;
    return AB;
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B) {
    A.reserve(A.size() + B.size());                // preallocate memory without erase original data
    A.insert(A.end(), B.begin(), B.end());         // add B;
    return A;                                      // here A could be named AB
}

double round_double(double var);
bool almost_eq(double a, double b, double allowed_diff);

#endif //GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_