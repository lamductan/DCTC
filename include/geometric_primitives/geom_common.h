#ifndef GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_
#define GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_

#include <cmath>
#include <iostream>
#include <vector>
#include <set>


const long double EPSILON = 1e-6L;
const int DECIMAL_DIGITS = 2;
const long double PI = acosl(-1L);
const long double TWO_PI = PI*2.0L;
const long double PI_2 = acosl(0L);
const long double PI_4 = PI_2/2.0L;
const long double PI_3 = acosl(1.0L/2.0L);
const long double TWO_PI_3 = acosl(-1.0L/2.0L);

bool approx(long double x, long double y); 
int findLargestAbsoluteValueCoefPos(const std::vector<long double>& vDouble);
long double rad2deg(long double rad);
long double deg2rad(long double deg);
long double computeGeometricAngle(long double angle);
long double standardize_angle(long double angle);

template<typename T>
std::ostream& print_vector(const std::vector<T>& v, char last_char=0, std::ostream& os=std::cout) {
    os << "[";
    for(const T& t : v) os << t << ", ";
    os << "]";
    if (last_char) os << last_char;
    return os;
}

template<typename T>
std::ostream& print_set(const std::set<T>& v, char last_char=0, std::ostream& os=std::cout) {
    os << "[";
    for(const T& t : v) os << t << ", ";
    os << "]";
    if (last_char) os << last_char;
    return os;
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

long double round_double(long double var);
bool almost_eq(long double a, long double b, long double allowed_diff);

#endif //GEOMETRIC_PRIMITIVES_GEOM_COMMON_H_