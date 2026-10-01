#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
int changing_middle(std::vector<int>& data) {
    if (data.empty() || data.size() % 2 == 0)
        throw std::invalid_argument("positive odd size required");
    std::sort(data.begin(), data.end()); // Changes caller storage.
    return data[data.size() / 2];
}
int preserving_middle(const std::vector<int>& data) {
    auto copy = data; // This value owns a separate vector.
    return changing_middle(copy);
}
void show(const std::vector<int>& data) {
    for (int x : data) std::cout << x << ' ';
    std::cout << '\n';
}
int main() {
    std::vector<int> changed{9, 1, 4};
    const std::vector<int> kept{9, 1, 4};
    std::cout << "changing=" << changing_middle(changed) << '\n';
    show(changed);
    std::cout << "preserving=" << preserving_middle(kept) << '\n';
    show(kept);
}
