// LAB: Represent private and shared mappings
// Use two maps whose entries own backing integers. Private mappings must point to distinct objects; one intentionally shared mapping must point to the same object in both maps.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <map>

int main() {
    std::map<unsigned, int> process_a{{0x1000, 7}};
    std::map<unsigned, int> process_b{{0x1000, 9}};
    process_a.at(0x1000) = 11;
    std::cout << process_a.at(0x1000) << ' ' << process_b.at(0x1000) << '\n';
    assert(process_b.at(0x1000) == 9);
}
