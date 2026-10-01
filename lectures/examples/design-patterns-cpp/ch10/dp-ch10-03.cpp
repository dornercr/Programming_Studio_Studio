#include <charconv>
#include <iostream>
#include <string>
int main() {
    const std::string token = "6x";
    int value = 0;
    const char* end = token.data() + token.size();
    const auto result = std::from_chars(token.data(), end, value);
    // Success of a prefix is weaker than success of the token.
    const bool prefix_ok = result.ec == std::errc{};
    const bool whole_ok = prefix_ok && result.ptr == end;
    std::cout << std::boolalpha;
    std::cout << "prefix=" << value << " conversion=" << prefix_ok << '\n';
    std::cout << "whole token=" << whole_ok << '\n';
    std::cout << (whole_ok ? "accepted" : "rejected") << '\n';
}
