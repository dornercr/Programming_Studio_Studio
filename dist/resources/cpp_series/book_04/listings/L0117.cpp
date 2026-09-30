#ifndef HARBOR_SERIES_CHECK_HPP
#define HARBOR_SERIES_CHECK_HPP
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string>

namespace harbor_test {
inline void check(bool passed, const char* expression,
                  std::source_location location = std::source_location::current()) {
    if (!passed) {
        throw std::runtime_error(std::string(location.file_name()) + ":" +
            std::to_string(location.line()) + ": check failed: " + expression);
    }
}
}
#define CHECK(...) ::harbor_test::check(static_cast<bool>((__VA_ARGS__)), #__VA_ARGS__)
#endif
