#include <iostream>
#include <vector>
int main(){int n=4;std::vector<std::vector<int>>list(n);std::vector<std::vector<bool>>matrix(n,std::vector<bool>(n));auto add=[&](int u,int v){list[u].push_back(v);matrix[u][v]=true;};add(0,1);add(0,3);std::cout<<list[0].size()<<' '<<matrix[0][3]<<'\n';}
