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

bool readCount(const std::string& text, int& output) {
    std::istringstream input(text);
    int candidate{};
    // Read a candidate, then check domain and complete consumption.
    if (!(input >> candidate) || candidate < 0 || candidate > 20) return false;
    input >> std::ws;
    if (!input.eof()) return false;
    output = candidate; // Commit only after every check succeeds.
    return true;
}

int main() {
    std::string text;
    std::getline(std::cin,text);
    int n=7;
    if(readCount(text,n))std::cout<<n<<"\n";
    else std::cout<<"invalid "<<n<<"\n";
    }
