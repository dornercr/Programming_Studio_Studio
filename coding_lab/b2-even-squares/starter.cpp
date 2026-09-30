#include <iostream>
#include <vector>
#include <ranges>

std::vector<int> evenSquares(const std::vector<int>& values){std::vector<int> result;for(int x:values)result.push_back(x*x);return result;}
