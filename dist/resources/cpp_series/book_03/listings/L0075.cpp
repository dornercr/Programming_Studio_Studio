#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
int main(){using P=std::pair<int,int>;std::vector<std::vector<P>>g(4);g[0]={{1,2},{2,5}};g[1]={{2,1},{3,5}};g[2]={{3,1}};const int INF=std::numeric_limits<int>::max()/4;std::vector<int>d(4,INF);std::priority_queue<P,std::vector<P>,std::greater<P>>q;d[0]=0;q.push({0,0});while(!q.empty()){auto [du,u]=q.top();q.pop();if(du!=d[u])continue;for(auto [v,w]:g[u])if(du+w<d[v]){d[v]=du+w;q.push({d[v],v});}}std::cout<<d[3]<<'\n';}
