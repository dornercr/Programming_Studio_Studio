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

int priceTotal(int unit, int count) {
    // A long intermediate holds this bounded product.
    // Check the int range before narrowing the return value.
    if (unit < 0 || unit > 1000 || count < 0 || count > 100) throw std::invalid_argument("range");
    long result = static_cast<long>(unit) * count;
    if (result > std::numeric_limits<int>::max()) throw std::out_of_range("overflow");
    return static_cast<int>(result);
}

int main() {
    int u,n;
    if(!(std::cin>>u>>n))return 2;
    try{std::cout<<priceTotal(u,n)<<"\n";
    }catch(const std::exception& e){std::cout<<e.what()<<"\n";
    }}
