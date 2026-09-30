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

bool acceptanceComplete(const std::vector<bool>& checks) {
    if (checks.empty()) return false; // Missing evidence is not acceptance.
    for (bool passed : checks) if (!passed) return false;
    return true;
}

int main() {
    std::cout<<std::boolalpha<<acceptanceComplete({})<<"\n";
    }
