#pragma once
#include <stdexcept>
#include <string>
#include <string_view>
namespace harbor {
inline void check(bool condition, std::string_view message) {
    if (!condition) throw std::runtime_error(std::string(message));
}
template<class F> void rejects(F&& f, std::string_view message) {
    bool rejected = false;
    try { f(); } catch (const std::exception&) { rejected = true; }
    check(rejected, message);
}
}
