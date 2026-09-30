#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>

bool validPath(const std::vector<std::vector<int>>& g,const std::vector<int>& p,int s,int t,std::size_t d){
    auto index=[&](int v){return v>=0&&std::size_t(v)<g.size();};
    if(!index(s)||!index(t)||p.empty()||p.front()!=s||p.back()!=t||p.size()-1!=d) return false;
    for(const auto& row:g) for(int v:row) if(!index(v)) return false;
    std::vector<bool> seen(g.size());
    for(std::size_t i=0;i<p.size();++i){
        int v=p[i];if(!index(v)||seen[v]) return false;seen[v]=true;
        if(i&&std::find(g[p[i-1]].begin(),g[p[i-1]].end(),v)==g[p[i-1]].end()) return false;
    }
    return true;
}
