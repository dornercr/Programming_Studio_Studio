#include <iostream>
#include <vector>
#include <cstddef>

bool eraseIndex(std::vector<int>& v,std::size_t i){if(i>=v.size())return false;v.pop_back();return true;}
