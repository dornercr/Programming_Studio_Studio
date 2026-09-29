#include <cassert>
#include <iostream>

int main() {
    const bool present = true;
    const bool readable = true;
    const bool writable = false;
    const auto allowed = [&](bool write) {
        return present && (write ? writable : readable);
    };
    std::cout << std::boolalpha << allowed(false) << ' ' << allowed(true) << '\n';
    assert(allowed(false) && !allowed(true));
}
