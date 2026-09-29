#pragma once
#include <charconv>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
namespace harbor {
inline constexpr std::int64_t max_input = 1'000'000;
struct Error { std::string code; std::string detail; };
template<class T> using Result = std::variant<T, Error>;
struct Request {
    std::string key;
    std::int64_t input{};
    bool operator==(const Request&) const = default;
};
inline bool valid_key(std::string_view key) {
    if (key.empty() || key.size() > 64) return false;
    for (unsigned char c : key)
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '_' || c == '-')) return false;
    return true;
}
inline Result<Request> parse_request(std::string_view key, std::string_view text) {
    if (!valid_key(key)) return Error{"INVALID", "key must be 1..64 ASCII identifier bytes"};
    std::int64_t n{};
    const auto [end, ec] = std::from_chars(text.data(), text.data()+text.size(), n);
    if (ec != std::errc{} || end != text.data()+text.size() || n < 0 || n > max_input)
        return Error{"INVALID", "input must be a whole integer in 0..1000000"};
    return Request{std::string(key), n};
}
inline std::int64_t evaluate(const Request& r) {
    if (!valid_key(r.key) || r.input < 0 || r.input > max_input)
        throw std::invalid_argument("invalid request");
    return r.input * r.input; // maximum is 10^12: within int64_t
}
}
