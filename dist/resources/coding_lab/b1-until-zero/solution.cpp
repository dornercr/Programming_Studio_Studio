#include <iostream>
#include <vector>

int positiveUntilZero(const std::vector<int>& readings){ int sum=0; for(int value:readings){ if(value==0)break; if(value<0)continue; sum+=value; } return sum; }
