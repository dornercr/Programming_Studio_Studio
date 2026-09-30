#include <iostream>
#include <vector>
#include <ranges>

std::vector<int> evenSquares(const std::vector<int>& values){auto selected=values|std::views::filter([](int x){return x%2==0;})|std::views::transform([](int x){return x*x;});std::vector<int> result;for(int x:selected)result.push_back(x);return result;}
