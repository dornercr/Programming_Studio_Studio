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

std::string band(int score) {
    if (score < 0 || score > 100) return "invalid";
    // Most specific threshold is checked first.
    if (score >= 80) return "high";
    if (score >= 50) return "pass";
    return "retry";
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    std::cout<<band(n)<<"\n";
    }
