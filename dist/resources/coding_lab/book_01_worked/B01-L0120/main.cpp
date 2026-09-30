// Original book listing B01-L0118
#ifndef HARBOR_GROUPS_HPP
#define HARBOR_GROUPS_HPP
namespace harbor {
int groups_needed(int items, int capacity);
}
#endif

// Original book listing B01-L0119
namespace harbor {
int groups_needed(int items, int capacity) {
    if (items < 0 || capacity <= 0) return -1;
    return items / capacity + (items % capacity != 0 ? 1 : 0);
}
}

// Original book listing B01-L0120
#include <iostream>

int main() {
    if (harbor::groups_needed(23, 5) != 5) return 1;
    if (harbor::groups_needed(0, 5) != 0) return 2;
    if (harbor::groups_needed(3, 0) != -1) return 3;
    std::cout << "groups=" << harbor::groups_needed(23, 5) << '\n';
    std::cout << "PASS\n";
}
