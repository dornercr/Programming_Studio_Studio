#include <iostream>
int main() {
    const unsigned count = 17u;
    const unsigned width = 16u;
    const unsigned exact = count * width; // 272 fits unsigned.
    const unsigned eight_bit_model = exact & 255u;
    const unsigned policy_limit = 64u;
    std::cout << "modeled narrow bytes=" << eight_bit_model << '\n';
    std::cout << std::boolalpha << "narrow check accepts="
              << (eight_bit_model <= policy_limit) << '\n';
    std::cout << "exact bytes=" << exact << '\n';
    std::cout << "correct check accepts="
              << (count <= policy_limit / width) << '\n';
}
