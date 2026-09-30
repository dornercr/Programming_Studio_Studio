#include <iostream>

const char message[] = "read-only";
int initialized = 7;
int zero_initialized;

void function_code() {}

int main() {
    std::cout << reinterpret_cast<const void*>(&function_code) << "\n";
    std::cout << static_cast<const void*>(message) << "\n";
    std::cout << &initialized << "\n";
    std::cout << &zero_initialized << "\n";
}
