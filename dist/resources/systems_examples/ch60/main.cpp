#include <cassert>
#include <iostream>
#include <string>

int main() {
    const std::string first = "12";
    const std::string second = "30";
    const int result = std::stoi(first) + std::stoi(second);
    const std::string line = std::to_string(result) + "\n";
    std::cout << line;
    assert(result == 42 && line == "42\n");
}
