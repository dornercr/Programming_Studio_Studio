#include <iostream>
#include <stdexcept>
#include <string>
std::string write_digit(int value) {
    if (value < 0 || value > 9)
        throw std::invalid_argument("digit");
    return std::to_string(value);
}
int read_digit(const std::string& wire) {
    // Check length before using a character as a digit.
    if (wire.size() != 1 || wire[0] < '0' || wire[0] > '9')
        throw std::invalid_argument("wire");
    return wire[0] - '0';
}
int main() {
    for (int value : {0, 4, 9}) {
        const auto wire = write_digit(value);
        std::cout << value << " -> " << wire << " -> "
                  << read_digit(wire) << '\n';
    }
}
