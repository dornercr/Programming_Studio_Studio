#include <iostream>
#include <queue>
#include <vector>
int main(){std::vector<std::vector<int>>g{{1,2},{3},{3},{}};std::vector<int>d(g.size(),-1);std::queue<int>q;d[0]=0;q.push(0);while(!q.empty()){int u=q.front();q.pop();for(int v:g[u])if(d[v]==-1){d[v]=d[u]+1;q.push(v);}}for(int x:d)std::cout<<x<<' ';}
