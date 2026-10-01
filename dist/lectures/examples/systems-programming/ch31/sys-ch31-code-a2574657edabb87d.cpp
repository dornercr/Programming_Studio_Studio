#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
int main() {
    std::vector<char> order; // Least recent at the front.
    int hits = 0, misses = 0;
    for (char block : std::string("ABCDABCD")) {
        auto found = std::find(order.begin(), order.end(), block);
        if (found != order.end()) {
            ++hits;
            order.erase(found);
        } else {
            ++misses;
            if (order.size() == 3) order.erase(order.begin());
        }
        order.push_back(block);
    }
    std::cout << "hits=" << hits << " misses=" << misses << '\n';
}
