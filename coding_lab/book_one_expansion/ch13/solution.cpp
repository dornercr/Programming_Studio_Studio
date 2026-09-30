#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

bool shiftAll(std::vector<int>& values, int delta) {
    // Validate the entire batch before the first write.
    for (int value : values) if (value + delta < -100 || value + delta > 100) return false;
    for (int& value : values) value += delta;
    return true;
}

int main() {
    std::vector<int> v{1,99};
    bool ok=shiftAll(v,2);
    std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\n";
    }
