#include <iostream>
#include <string>
int main() {
    std::string name;
    bool accepted = false;
    bool enabled = false;
    // Wrong: each handler replaces the combined policy.
    enabled = accepted;
    name = "Mina";
    enabled = !name.empty();
    std::cout << std::boolalpha;
    std::cout << "split handlers: " << enabled << '\n';
    enabled = !name.empty() && accepted;
    std::cout << "combined rule: " << enabled << '\n';
}
