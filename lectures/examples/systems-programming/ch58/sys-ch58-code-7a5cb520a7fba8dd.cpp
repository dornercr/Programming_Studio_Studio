#include <iostream>
enum class Role { parser, maintenance };
enum class Action { read, erase };
bool allowed(Role role, Action action) {
    return action == Action::read || role == Role::maintenance;
}
int main() {
    std::cout << "parser read=" << allowed(Role::parser, Action::read) << '\n';
    std::cout << "parser delete=" << allowed(Role::parser, Action::erase) << '\n';
    std::cout << "maintenance delete=" << allowed(Role::maintenance, Action::erase) << '\n';
}
