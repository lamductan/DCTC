#ifndef DATA_STRUCTURES_UNION_FIND_H_
#define DATA_STRUCTURES_UNION_FIND_H_

#include <vector>


class UnionFind {
private:
    int size;
    std::vector<int> parent;
    std::vector<int> rk;
public:
    UnionFind(int n);
    int find(int x);
    void join(int x, int y);
};

#endif //DATA_STRUCTURES_UNION_FIND_H_