// Exact original book listing B02-L0110
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

// Original book listing B02-L0081
#include <charconv>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>

std::optional<int> integer(std::string_view text) {
    if (text.empty()) return std::nullopt;
    int value{};
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), value);
    if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size()) {
        return std::nullopt;
    }
    return value;
}
struct Reading { int value; };
struct Error { std::string message; };
using Result = std::variant<Reading, Error>;
Result parse(std::string_view text) {
    const auto value = integer(text);
    if (!value) return Error{"invalid integer"};
    return Reading{*value};
}

int main() {
    CHECK(integer("0").has_value());
    CHECK(!integer("12ms") && !integer(""));
    CHECK(std::get<Reading>(parse("42")).value == 42);
    CHECK(std::holds_alternative<Error>(parse("bad")));
    const std::string report = std::visit([](const auto& item) -> std::string {
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_same_v<T, Reading>) return std::to_string(item.value);
        else return item.message;
    }, parse("42"));
    CHECK(report == "42");
    auto summary = std::tuple{3, 12, std::string{"north"}};
    const auto& [count, sum, name] = summary;
    CHECK(count == 3 && sum == 12 && name == "north");
    course::report();
}
