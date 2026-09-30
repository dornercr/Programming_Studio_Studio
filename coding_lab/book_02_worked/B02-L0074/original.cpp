#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>
int main(){ std::vector<int> v{4,1,3,2}; std::ranges::sort(v); for(int x:v)std::cout<<x<<' '; }
