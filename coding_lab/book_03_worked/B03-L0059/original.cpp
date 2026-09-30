#include <iostream>
#include <vector>
void dfs(int u,const std::vector<std::vector<int>>&g,std::vector<bool>&seen){seen[u]=true;std::cout<<u<<' ';for(int v:g[u])if(!seen[v])dfs(v,g,seen);}int main(){std::vector<std::vector<int>>g{{1,2},{3},{},{}};std::vector<bool>seen(g.size());dfs(0,g,seen);}
