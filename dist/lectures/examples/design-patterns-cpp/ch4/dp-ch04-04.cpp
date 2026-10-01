#include <iostream>
#include <memory>
#include <string>
#include <vector>
int main() {
    auto original = std::make_shared<std::vector<std::string>>(
        std::initializer_list<std::string>{"Check", "Begin"});
    auto handle_copy = original; // Shares the same mutable list.
    handle_copy->front() = "Inspect";
    std::cout << "original after handle edit="
              << original->front() << '\n';
    auto value_copy = std::make_shared<std::vector<std::string>>(
        *original); // Copy the list itself into a new object.
    value_copy->front() = "Continue";
    std::cout << "original after value edit="
              << original->front() << '\n';
    std::cout << "new list=" << value_copy->front() << '\n';
}
