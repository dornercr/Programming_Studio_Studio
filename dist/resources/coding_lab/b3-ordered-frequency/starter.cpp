#include <iostream>
#include <vector>
#include <map>
#include <cstddef>

std::map<int,std::size_t> frequencies(const std::vector<int>& v){std::map<int,std::size_t> m;for(int x:v)m[x]=1;return m;}
