#include <iostream>
#include <vector>
#include <algorithm>

void eraseNegative(std::vector<int>& values){auto p=std::find_if(values.begin(),values.end(),[](int n){return n<0;});if(p!=values.end())values.erase(p);}
