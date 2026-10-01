#include <iostream>
#include <memory>
#include <utility>
int main() {
    // One-thread ownership model: no concurrent queue is claimed.
    std::unique_ptr<int> slot;
    bool closed = false;
    const auto push = [&](std::unique_ptr<int>& item) {
        if (closed || slot || !item) return false;
        slot = std::move(item);
        return true;
    };
    auto first = std::make_unique<int>(7);
    auto second = std::make_unique<int>(9);
    const bool accepted = push(first);
    const bool full_rejected = !push(second);
    closed = true;
    auto drained = std::move(slot);
    const bool closed_rejected = !push(second);
    std::cout << std::boolalpha << "accepted=" << accepted
              << " caller-empty=" << !first << '\n';
    std::cout << "full-rejected=" << full_rejected << " retained=" << *second << '\n';
    std::cout << "drained=" << *drained << " closed-rejected=" << closed_rejected << '\n';
}
