#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <algorithm>

std::map<int,unsigned> outcomes() {
    std::array<int,4> order{0,0,1,1};
    std::map<int,unsigned> counts;
    do {
        int counter = 0;
        std::array<int,2> local{0,0}, step{0,0};
        for(int who : order) {
            if(step[who]++ == 0) local[who] = counter;
            else counter = local[who]+1;
        }
        ++counts[counter];
    } while(std::next_permutation(order.begin(),order.end()));
    return counts;
}

int main() {
    const auto counts = outcomes();
    assert(counts.at(1) == 4 && counts.at(2) == 2);
    std::cout << "lost=" << counts.at(1) << " serialized=" << counts.at(2) << '\n';
}
