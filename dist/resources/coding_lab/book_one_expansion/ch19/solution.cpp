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

namespace billing {
    int subtotal(int unit, int count); // Public declaration.
}
int billing::subtotal(int unit, int count) {
    if (unit < 0 || unit > 100 || count < 0 || count > 100) throw std::invalid_argument("range");
    return unit * count;
}

int main() {
    int u,n;
    if(!(std::cin>>u>>n))return 2;
    try{std::cout<<billing::subtotal(u,n)<<"\n";
    }catch(const std::invalid_argument&){std::cout<<"range\n";
    }}
