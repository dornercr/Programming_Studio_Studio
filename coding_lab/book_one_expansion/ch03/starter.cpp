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
    // BUG: a successful numeric prefix is accepted.
    return static_cast<bool>(input >> output);
}

int main() {
    std::string text;
    std::getline(std::cin,text);
    int n=7;
    if(readCount(text,n))std::cout<<n<<"\n";
    else std::cout<<"invalid "<<n<<"\n";
    }
