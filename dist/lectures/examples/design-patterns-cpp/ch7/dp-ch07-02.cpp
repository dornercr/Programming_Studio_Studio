#include <iostream>
#include <stdexcept>
#include <string>

std::string count_text(int n) {
    if (n < 0) throw std::invalid_argument("count");
    return "count=" + std::to_string(n); // One policy, one format.
}
int main() {
    std::cout << count_text(6) << '\n';
    std::cout << count_text(0) << '\n';
    try { std::cout << count_text(-1) << '\n'; }
    catch (const std::invalid_argument&) { std::cout << "negative rejected\n"; }
}
