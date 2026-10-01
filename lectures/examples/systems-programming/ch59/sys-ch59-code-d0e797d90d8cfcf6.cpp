#include <charconv>
#include <iostream>
#include <string>
#include <system_error>
int main() {
    const std::string input = "80x";
    int value = 0;
    const auto parsed = std::from_chars(input.data(), input.data() + input.size(), value);
    const bool complete = parsed.ec == std::errc{} && parsed.ptr == input.data() + input.size();
    std::cout << "prefix=" << value << " consumed=" << parsed.ptr - input.data() << '\n';
    std::cout << "complete=" << complete << '\n';
}
