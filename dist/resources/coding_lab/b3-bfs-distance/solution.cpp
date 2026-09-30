#include <iostream>
#include <vector>
#include <queue>
#include <cstddef>
#include <stdexcept>

std::vector<int> distances(const std::vector<std::vector<int>>& g,int s){
    if(s<0||std::size_t(s)>=g.size()) throw std::invalid_argument("source");
    for(const auto& row:g) for(int v:row)
        if(v<0||std::size_t(v)>=g.size()) throw std::invalid_argument("edge");
    std::vector<int> d(g.size(),-1);std::queue<int> pending;
    d[s]=0;pending.push(s);
    while(!pending.empty()){
        int u=pending.front();pending.pop();
        for(int v:g[u]) if(d[v]==-1){d[v]=d[u]+1;pending.push(v);}
    }
    return d;
}
