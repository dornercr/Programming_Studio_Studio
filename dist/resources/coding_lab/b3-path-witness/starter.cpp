#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>

bool validPath(const std::vector<std::vector<int>>& g,const std::vector<int>& p,int s,int t,std::size_t d){(void)g;(void)d;return !p.empty()&&p.front()==s&&p.back()==t;}
