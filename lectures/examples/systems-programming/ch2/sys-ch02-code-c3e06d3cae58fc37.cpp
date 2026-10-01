#include <cstring>
#include <iostream>
#include <span>
int main() {
    const char packet[]{'A', '\0', 'B', '\0'};
    const std::span<const char> payload(packet, 3);
    std::cout << "string length=" << std::strlen(packet) << '\n';
    std::cout << "payload length=" << payload.size() << '\n';
    std::cout << "third byte=" << payload[2] << '\n';
}
