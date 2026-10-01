#include <iostream>
#include <string>
int main() {
    const std::string input = "0123456789";
    std::string batch, received;
    int calls = 0;
    const auto flush = [&] {
        if (batch.empty()) return;
        received += batch; // Model one sink call, preserving byte order.
        ++calls;
        batch.clear();
    };
    for (char item : input) {
        batch += item;
        if (batch.size() == 4) flush();
    }
    flush();
    if (received != input) return 1;
    std::cout << "per-item calls=" << input.size() << " batched calls=" << calls << '\n';
    std::cout << "received=" << received << '\n';
}
