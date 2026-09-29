#include <iostream>
#include <iterator>

int main() {
    int readings[]{3, 6, 9};
    int* selected = nullptr;
    const std::size_t index{1};
    if (index < std::size(readings)) selected = &readings[index];
    if (selected == nullptr) return 1;
    *selected += 1;
    int sum{};
    const int* begin = readings;
    const int* end = readings + std::size(readings);
    for (const int* current = begin; current != end; ++current) {
        sum += *current;
    }
    if (readings[1] != 7 || sum != 19) return 2;
    std::cout << "sum=" << sum << '\n';
    std::cout << "PASS\n";
}
