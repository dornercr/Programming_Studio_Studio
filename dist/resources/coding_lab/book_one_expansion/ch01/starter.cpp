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

std::string stageName(int stage) {
    // BUG: every stage gets the same name.
    return stage == 0 ? "preprocess" : "compile";
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    std::cout<<stageName(n)<<"\n";
    }
