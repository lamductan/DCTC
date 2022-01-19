#ifndef DCTC_UTILS_TPP_
#define DCTC_UTILS_TPP_


#include <iostream>
#include <vector>
#include <unordered_map>

template<typename K, typename V>
std::vector<K> getMapKeys(const std::unordered_map<K, V>& m) {
    std::vector<K> keys;
    for(auto it : m) keys.push_back(it.first);
    return keys;
}

template<typename K, typename V>
std::vector<V> getMapValues(const std::unordered_map<K, V>& m) {
    std::vector<V> values;
    for(auto it : m) values.push_back(it.second);
    return values;
}

template<typename T>
void print_vector_ptr(const std::vector<T*>& v, char last_char, std::ostream& os) {
    os << "[";
    for(const T* t : v) os << *t << ", ";
    os << "]";
    if (last_char) os << last_char;
}

#endif // DCTC_UTILS_TPP_
