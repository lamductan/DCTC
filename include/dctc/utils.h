#ifndef DCTC_UTILS_H_
#define DCTC_UTILS_H_

#include <iostream>
#include <vector>
#include <unordered_map>


template<typename K, typename V>
std::vector<K> getMapKeys(const std::unordered_map<K, V>& m);

template<typename K, typename V>
std::vector<V> getMapValues(const std::unordered_map<K, V>& m);

template<typename T>
std::ostream& print_vector_ptr(const std::vector<T*>& v, char last_char=0, std::ostream& os=std::cout);

template<typename T>
std::ostream& print_vector_ptr_new_line(const std::vector<T*>& v, char last_char=0, std::ostream& os=std::cout);

#include "dctc/utils.tpp"

#endif // DCTC_UTILS_H_
