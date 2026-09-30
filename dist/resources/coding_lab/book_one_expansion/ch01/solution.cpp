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
    // Reject a label outside the four modeled stages.
    if (stage < 0 || stage > 3) return "invalid";
    if (stage == 0) return "preprocess";
    if (stage == 1) return "compile";
    if (stage == 2) return "assemble";
    return "link";
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    std::cout<<stageName(n)<<"\n";
    }
