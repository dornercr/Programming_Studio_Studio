#include <iostream>
#include <stdexcept>
struct Guard {
    ~Guard() { std::cout << "cleanup\n"; }
};
int main() {
    int completed = 0;
    try {
        Guard guard;
        std::cout << "begin\n";
        ++completed;
        throw std::runtime_error("controlled failure");
    } catch (const std::runtime_error&) {
        std::cout << "caught completed=" << completed << '\n';
    }
}
