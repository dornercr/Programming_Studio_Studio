#include <climits>
#include <iostream>
#include <limits>

int main() {
    std::cout << "char bits=" << CHAR_BIT << "\n";
    std::cout << "int digits=" << std::numeric_limits<int>::digits << "\n";
    unsigned x = std::numeric_limits<unsigned>::max();
    std::cout << "unsigned wrap=" << (x + 1u) << "\n";
}
