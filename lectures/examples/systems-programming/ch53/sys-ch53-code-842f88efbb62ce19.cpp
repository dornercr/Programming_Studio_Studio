#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
int main() {
    const std::string response = "hello";
    std::string accepted;
    std::size_t offset = 0;
    for (std::size_t count : std::array<std::size_t, 3>{2, 1, 2}) {
        if (count > response.size() - offset) throw std::logic_error("bad fixture");
        accepted.append(response, offset, count);
        offset += count; // Advance only by accepted bytes.
    }
    std::cout << "sent=" << accepted << " pending=" << response.size() - offset << '\n';
}
