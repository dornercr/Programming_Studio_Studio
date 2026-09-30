#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<int> above(const std::vector<int>& values,int threshold){std::vector<int> result;std::copy_if(values.begin(),values.end(),std::back_inserter(result),[threshold](int x){return x>threshold;});return result;}
