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

bool transfer(std::vector<int>& stock, std::size_t from, std::size_t to, int amount) {
    if (from >= stock.size() || to >= stock.size() || amount < 0) return false;
    if (amount > stock[from]) return false;
    if (from == to) return true; // No net movement, after availability validation.
    if (amount > 1000 - stock[to]) return false;
    stock[from] -= amount;
    stock[to] += amount;
    return true;
}

int main() {
    std::vector<int> v{10,999};
    bool ok=transfer(v,0,1,2);
    std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\n";
    }
