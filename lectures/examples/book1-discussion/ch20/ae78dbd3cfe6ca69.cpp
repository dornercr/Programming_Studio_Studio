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

struct Change { std::size_t index; int delta; };
bool applyBatch(std::vector<int>& stock, const std::vector<Change>& changes) {
    auto candidate = stock; // Original remains intact during validation.
    for (const auto& change : changes) {
        if (change.index >= candidate.size()) return false;
        int next = candidate[change.index] + change.delta;
        if (next < 0 || next > 1000) return false;
        candidate[change.index] = next;
    }
    stock.swap(candidate); // Commit the fully checked batch.
    return true;
}

int main() {
    std::vector<int> v{5,2};
    bool ok=applyBatch(v,{{0,-1},{1,-3}});
    std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\n";
    }
