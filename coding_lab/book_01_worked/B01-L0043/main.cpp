#include <iostream>

int main() {
    const int items{23};
    const int capacity{5};
    if (items < 0 || capacity <= 0) return 2;
    const int full_groups = items / capacity;
    const int remaining = items % capacity;
    const bool needs_partial_group = remaining != 0;
    const int groups = full_groups + (needs_partial_group ? 1 : 0);
    std::cout << "full=" << full_groups
              << " remaining=" << remaining
              << " total=" << groups << '\n';
    if (full_groups != 4 || remaining != 3 || groups != 5) return 1;
    std::cout << "PASS\n";
}
