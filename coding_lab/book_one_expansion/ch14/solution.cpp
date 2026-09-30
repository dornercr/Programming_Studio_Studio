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

int* firstAbove(std::vector<int>& values, int threshold) {
    for (int& value : values) {
        if (value > threshold) return &value; // Borrow the actual element.
    }
    return nullptr;
}

int main() {
    std::vector<int> v{4,4,9};
    int* p=firstAbove(v,4);
    if(p)std::cout<<*p<<"\n";
    else std::cout<<"none\n";
    }
