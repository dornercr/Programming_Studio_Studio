// LAB: Build a byte-summary component with a clear contract
// Summarize a string of up to one million bytes by its byte count and unsigned-byte sum. Treat embedded zero bytes as data and reject oversized input.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
