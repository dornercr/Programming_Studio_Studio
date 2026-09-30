#include <iostream>
#include <vector>
#include <algorithm>

void eraseNegative(std::vector<int>& values){values.erase(std::remove_if(values.begin(),values.end(),[](int n){return n<0;}),values.end());}
