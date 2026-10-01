#include <iostream>
#include <string>
bool ready(const std::string& name, bool accepted) {
    // One function owns the whole readiness rule.
    return !name.empty() && accepted;
}
int main() {
    std::cout << std::boolalpha;
    std::cout << ready("Mina", false) << '\n';
    std::cout << ready("Mina", true) << '\n';
    std::cout << ready("", true) << '\n';
}
