#include <iostream>
#include <list>
#include <vector>
int main(){std::vector<std::list<int>>b(3);for(int x:{1,4,7})b[x%3].push_back(x);for(std::size_t i=0;i<b.size();++i){std::cout<<i<<":";for(int x:b[i])std::cout<<x<<',';std::cout<<'\n';}}
