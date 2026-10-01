#include <cstddef>
#include <iostream>
std::size_t cost(std::size_t records, std::size_t batch) {
    // The caller supplies a positive batch size in this small model.
    const auto batches = records / batch + (records % batch != 0);
    return batches * 5 + records * 2;
}
int main() {
    const std::size_t records = 10;
    std::cout << "one-record batches=" << cost(records, 1) << '\n';
    std::cout << "up-to-four batches=" << cost(records, 4) << '\n';
    std::cout << "empty work=" << cost(0, 4) << '\n';
}
