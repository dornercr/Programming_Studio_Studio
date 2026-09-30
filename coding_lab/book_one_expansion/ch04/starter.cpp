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

int checkedNext(int value) {
    // BUG: valid inputs never increase.
    if(value==std::numeric_limits<int>::max())throw std::out_of_range("overflow");
    return value;
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    try{std::cout<<checkedNext(n)<<"\n";
    }catch(const std::out_of_range&){std::cout<<"overflow\n";
    }}
