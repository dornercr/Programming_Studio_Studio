#include <iostream>
struct Permissions { bool read; bool write; bool execute; };
bool least_permission(const Permissions& p) {
    return !(p.write && p.execute);
}
int main() {
    const Permissions data{true, true, false};
    const Permissions code{true, false, true};
    const Permissions writable_code{true, true, true};
    std::cout << "data-ok=" << least_permission(data) << '\n';
    std::cout << "code-ok=" << least_permission(code) << '\n';
    std::cout << "writable-code-ok=" << least_permission(writable_code) << '\n';
}
