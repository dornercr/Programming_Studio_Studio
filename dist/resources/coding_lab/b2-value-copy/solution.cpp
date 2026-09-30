#include <iostream>
#include <vector>

std::vector<int> shifted(const std::vector<int>& readings,int delta){auto result=readings;for(int& value:result)value+=delta;return result;}
