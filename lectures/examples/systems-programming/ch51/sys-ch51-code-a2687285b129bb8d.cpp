#include <algorithm>
#include <array>
#include <iostream>
#include <string>
int main() {
    const std::string payload = "A\nB";
    const std::string delimited = payload + '\n';
    std::cout << "delimiter messages="
              << std::count(delimited.begin(), delimited.end(), '\n') << '\n';
    const std::array<unsigned char, 5> frame{0, 3, 'A', '\n', 'B'};
    const unsigned length = (unsigned(frame[0]) << 8) | frame[1];
    std::cout << "length payload bytes=" << length << '\n';
}
