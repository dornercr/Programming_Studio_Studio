#include <iostream>
#include <string>
#include <vector>
int main() {
    std::string pending;
    for (const std::string& chunk : std::vector<std::string>{"HE", "LLO\nBY", "E\n"}) {
        pending += chunk; // Model byte arrivals, not TCP packet boundaries.
        std::size_t end;
        while ((end = pending.find('\n')) != std::string::npos) {
            std::cout << pending.substr(0, end) << '\n';
            pending.erase(0, end + 1);
        }
    }
    std::cout << "tail=" << pending.size() << '\n';
}
