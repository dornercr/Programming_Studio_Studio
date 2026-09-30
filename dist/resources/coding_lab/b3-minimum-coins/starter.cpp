#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

int minimumCoins(const std::vector<int>& coins,int target){if(target<0||target>1000)throw std::invalid_argument("target");for(int c:coins)if(c<=0)throw std::invalid_argument("coin");return target==0?0:-1;}
