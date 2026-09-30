#include "check.hpp"
#include <array>
#include <charconv>
#include <optional>
#include <string>
#include <string_view>

std::optional<unsigned> parse_port(std::string_view text) {
    if (text.empty() || text.size() > 5) return std::nullopt;
    for (char character : text)
        if (character < '0' || character > '9') return std::nullopt;
    unsigned value{};
    const auto [end, error] = std::from_chars(text.data(), text.data()+text.size(), value);
    if (error != std::errc{} || end != text.data()+text.size() || value > 65535) return std::nullopt;
    return value;
}
int main() {
    struct Case { std::string_view text; std::optional<unsigned> expected; };
    const std::array cases{
        Case{"0", 0}, Case{"1", 1}, Case{"65535", 65535}, Case{"00001", 1},
        Case{"65536", std::nullopt}, Case{"", std::nullopt}, Case{"-1", std::nullopt},
        Case{"+1", std::nullopt}, Case{"1x", std::nullopt}, Case{" 1", std::nullopt},
        Case{"000001", std::nullopt}
    };
    for (const auto& item : cases) CHECK(parse_port(item.text) == item.expected);
    for (unsigned value = 0; value <= 65535; value += 31)
        CHECK(parse_port(std::to_string(value)) == value);
    std::cout << "table_cases=" << cases.size() << "\nPASS\n";
}
