#include "data_structures/union_find.h"


UnionFind::UnionFind(int n) {
    size = n;
    parent.assign(n, 0);
    rk.assign(n, 0);
    for(int i = 0; i < n; ++i) parent[i] = i;
}

int UnionFind::find(int x) {
    if (x == parent[x]) return x;
    return parent[x] = find(parent[x]);
}

void UnionFind::join(int x, int y) {
    x = find(x);
    y = find(y);
    if (rk[x] < rk[y]) std::swap(x, y);
    parent[y] = x;
    if (rk[x] == rk[y]) ++rk[x];
}