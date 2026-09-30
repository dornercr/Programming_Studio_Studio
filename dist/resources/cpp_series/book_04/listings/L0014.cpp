#include <iostream>
#include <memory>

int main() {
    int stack_value = 10;
    auto heap_value = std::make_unique<int>(20);
    std::cout << "stack=" << &stack_value << "\n";
    std::cout << "heap=" << heap_value.get() << "\n";
}
