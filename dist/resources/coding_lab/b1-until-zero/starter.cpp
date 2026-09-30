#include <iostream>
#include <vector>

int positiveUntilZero(const std::vector<int>& readings){ int sum=0; for(int value:readings)sum+=value; return sum; }
