#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <optional>
#include <cstddef>
#include <stdexcept>

std::size_t selectCount(std::vector<std::pair<int,int>> v){for(auto [a,b]:v)if(a>=b)throw std::invalid_argument("interval");std::sort(v.begin(),v.end());std::optional<int> end;std::size_t n=0;for(auto [a,b]:v)if(!end||a>=*end){++n;end=b;}return n;}
