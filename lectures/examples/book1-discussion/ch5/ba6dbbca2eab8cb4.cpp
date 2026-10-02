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

std::pair<int,int> packing(int items, int group) {
    // Positive group size prevents division by zero.
    if (items < 0 || items > 1000 || group < 1 || group > 100) throw std::invalid_argument("range");
    return {items / group, items % group};
}

int main() {
    int n,g;
    if(!(std::cin>>n>>g))return 2;
    try{auto p=packing(n,g);
    std::cout<<p.first<<" "<<p.second<<"\n";
    }catch(const std::invalid_argument&){std::cout<<"range\n";
    }}
