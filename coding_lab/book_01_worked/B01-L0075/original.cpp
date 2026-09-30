#include <charconv>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

bool parse_id(std::string_view text, int& result) {
    if (text.empty()) return false;
    int candidate{};
    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto parsed = std::from_chars(begin, end, candidate);
    if (parsed.ec != std::errc{} || parsed.ptr != end) return false;
    if (candidate < 1 || candidate > 1000000) return false;
    result = candidate;
    return true;
}

int main() {
    const std::string field{"sensor=42"};
    const auto separator = field.find('=');
    if (separator == std::string::npos) return 1;
    const std::string name = field.substr(0, separator);
    const std::string value = field.substr(separator + 1);
    int id{-1};
    if (name != "sensor" || !parse_id(value, id) || id != 42) return 2;
    if (parse_id("42junk", id) || parse_id(" 42", id)) return 3;
    if (parse_id("", id) || parse_id("0", id)) return 4;
    if (id != 42) return 5; // Failed parses preserve the previous result.
    std::cout << name + ":" + std::to_string(id) << '\n';
    std::cout << "PASS\n";
}
