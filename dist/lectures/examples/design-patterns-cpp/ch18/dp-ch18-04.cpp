#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
int main() {
    int first = 0;
    int second = 0;
    std::vector<std::function<void(int)>> listeners;
    listeners.push_back([&](int value) {
        first += value;
        throw std::runtime_error("view failed");
    });
    listeners.push_back([&](int value) { second += value; });
    try {
        for (const auto& update : listeners) { update(5); }
    } catch (const std::runtime_error&) {
        std::cout << "delivery stopped\n";
    }
    std::cout << "first: " << first << '\n';
    std::cout << "second: " << second << '\n';
}
