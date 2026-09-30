#include <iostream>
#include <vector>
int main(){std::vector<std::vector<int>>g(4);g[0]={1,2};g[1]={3};for(std::size_t u=0;u<g.size();++u)for(int v:g[u])std::cout<<u<<"->"<<v<<'\n';}
