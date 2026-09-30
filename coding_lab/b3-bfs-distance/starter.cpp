#include <iostream>
#include <vector>
#include <queue>
#include <cstddef>
#include <stdexcept>

std::vector<int> distances(const std::vector<std::vector<int>>& g,int s){if(s<0||std::size_t(s)>=g.size())throw std::invalid_argument("source");for(auto& row:g)for(int v:row)if(v<0||std::size_t(v)>=g.size())throw std::invalid_argument("edge");std::vector<int> d(g.size(),-1);d[s]=0;return d;}
