#include <iostream>
#include <stdexcept>
#include <string>
std::string framed_write(int value) {
    if (value < 0 || value > 9)
        throw std::invalid_argument("digit");
    return "[" + std::to_string(value) + "]";
}
int plain_read(const std::string& wire) {
    if (wire.size() != 1 || wire[0] < '0' || wire[0] > '9')
        throw std::invalid_argument("plain needs one digit");
    return wire[0] - '0';
}
int main() {
    const auto wire = framed_write(4);
    std::cout << "sent=" << wire << '\n';
    try { plain_read(wire); }
    catch (const std::invalid_argument&) {
        std::cout << "receiver rejected the family\n";
    }
    // A valid plain input confirms that the reader itself works.
    std::cout << "plain control=" << plain_read("4") << '\n';
}
