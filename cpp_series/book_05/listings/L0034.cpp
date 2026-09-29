#include "check.hpp"
#include <charconv>
#include <optional>
#include <string_view>
std::optional<unsigned> parse_count(std::string_view text) {
    unsigned value{};
    const auto [end, error] = std::from_chars(text.data(), text.data()+text.size(), value);
    if (error != std::errc{} || end != text.data()+text.size() || value > 1000) return std::nullopt;
    return value;
}
int main() {
    CHECK(parse_count("0") == 0);
    CHECK(parse_count("1000") == 1000);
    CHECK(!parse_count("1001") && !parse_count("12x") && !parse_count("-1"));
    std::cout << "PASS\n";
}
