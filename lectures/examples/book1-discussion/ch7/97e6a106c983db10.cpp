#include <array>
#include <iostream>

int main() {
    const std::array<int, 7> readings{4, -2, 7, 0, 9, 999, 50};
    int sum{};
    int accepted{};
    for (int value : readings) {
        if (value == 999) break; // Teaching end marker.
        if (value < 0) continue;
        sum += value;
        ++accepted;
    }
    std::cout << "accepted=" << accepted << " sum=" << sum << '\n';
    if (accepted != 4 || sum != 20) return 1;
    std::cout << "PASS\n";
}
