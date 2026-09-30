#include <iostream>
#include <vector>

std::vector<int> prefixMax(const std::vector<int>& values){ int best=0;std::vector<int> out;for(int x:values){if(x>best)best=x;out.push_back(best);}return out; }
