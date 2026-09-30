#include "groups.hpp"
#include "groups.hpp" // The guard makes repeated inclusion harmless here.
#include <iostream>

int main() {
    if (harbor::groups_needed(23, 5) != 5) return 1;
    if (harbor::groups_needed(0, 5) != 0) return 2;
    if (harbor::groups_needed(3, 0) != -1) return 3;
    std::cout << "groups=" << harbor::groups_needed(23, 5) << '\n';
    std::cout << "PASS\n";
}
