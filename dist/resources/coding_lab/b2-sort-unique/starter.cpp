#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> sortedUnique(std::vector<int> values){auto end=std::unique(values.begin(),values.end());values.erase(end,values.end());return values;}
