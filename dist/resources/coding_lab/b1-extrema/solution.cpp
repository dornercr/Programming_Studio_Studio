#include <iostream>
#include <array>
#include <utility>

std::pair<int,int> extrema(const std::array<int,4>& values){ int low=values[0],high=values[0]; for(int x:values){if(x<low)low=x;if(x>high)high=x;}return {low,high}; }
