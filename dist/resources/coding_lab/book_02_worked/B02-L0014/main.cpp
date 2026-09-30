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

// Original book listing B02-L0014
#include <cstdio>
#include <stdexcept>
#include <string>

class TemporaryFile {
    std::FILE* file_;
public:
    TemporaryFile() : file_(std::tmpfile()) {
        if (!file_) throw std::runtime_error("tmpfile failed");
    }
    ~TemporaryFile() { if (file_) std::fclose(file_); }
    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile& operator=(const TemporaryFile&) = delete;
    std::FILE* get() const noexcept { return file_; }
    bool close() noexcept {
        std::FILE* old = file_;
        file_ = nullptr;
        return !old || std::fclose(old) == 0;
    }
};
class ActiveScope {
    int& live_;
public:
    explicit ActiveScope(int& live) : live_(live) { ++live_; }
    ~ActiveScope() { --live_; }
    ActiveScope(const ActiveScope&) = delete;
    ActiveScope& operator=(const ActiveScope&) = delete;
};

struct SimulatedFailure final {};

int main() {
    TemporaryFile file;
    const std::string text{"resource owned\n"};
    CHECK(std::fwrite(text.data(), 1, text.size(), file.get()) == text.size());
    CHECK(std::fseek(file.get(), 0, SEEK_SET) == 0);
    char buffer[64]{};
    const auto bytes = std::fread(buffer, 1, sizeof buffer, file.get());
    CHECK(std::string(buffer, bytes) == text);
    CHECK(file.close());
    int live{};
    try {
        ActiveScope scope{live};
        CHECK(live == 1);
        throw SimulatedFailure{};
    } catch (const SimulatedFailure&) {}
    CHECK(live == 0);
    course::report();
}
