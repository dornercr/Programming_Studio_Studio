#include <charconv>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <string_view>
#include <system_error>
std::optional<unsigned> count_from(std::string_view text) {
    unsigned value = 0;
    const auto result = std::from_chars(text.data(), text.data()+text.size(), value);
    if (result.ec != std::errc{} || result.ptr != text.data()+text.size()
        || value < 1 || value > 64) return std::nullopt;
    return value;
}
int main() {
    for (const auto text : {"12", "12x", "0", "-1"}) {
        const auto value = count_from(text);
        std::cout << text << " -> ";
        if (value) std::cout << *value << '\n';
        else std::cout << "rejected\n";
    }
}
