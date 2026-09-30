#include <iostream>
#include <vector>

std::vector<int> prefixMax(const std::vector<int>& values){ std::vector<int> out;out.reserve(values.size());for(int x:values){if(out.empty()||x>out.back())out.push_back(x);else out.push_back(out.back());}return out; }
