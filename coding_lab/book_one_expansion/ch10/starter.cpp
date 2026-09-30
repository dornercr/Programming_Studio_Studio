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

std::vector<int> rowTotals(const std::array<std::array<int,3>,2>& grid){
 std::vector<int> out;
 // BUG: visits only the first two columns.
 for(const auto& row:grid)out.push_back(row[0]+row[1]);
 return out;
}

int main() {
    std::array<std::array<int,3>,2> g{};
    for(auto& row:g)for(int& x:row)if(!(std::cin>>x))return 2;
    auto v=rowTotals(g);
    std::cout<<v[0]<<" "<<v[1]<<"\n";
    }
