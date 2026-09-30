#include <iostream>
#include <vector>

int countPositive(const std::vector<int>& values) {
    int count=0;
    for (int value : values) if (value>0) ++count;
    return count;
}
