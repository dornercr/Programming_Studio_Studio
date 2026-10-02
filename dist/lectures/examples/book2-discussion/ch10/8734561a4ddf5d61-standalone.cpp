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

// Original book listing B02-L0044
#include <string>
#include <vector>
#include <utility>

struct Sink {
    virtual ~Sink() = default;
    virtual void write(const std::string& text) = 0;
};
class MemorySink final : public Sink {
public:
    std::vector<std::string> messages;
    void write(const std::string& text) override { messages.push_back(text); }
};
class Formatter {
    std::string prefix_;
public:
    explicit Formatter(std::string prefix) : prefix_(std::move(prefix)) {}
    std::string format(int value) const { return prefix_ + std::to_string(value); }
};
class Reporter {
    Formatter formatter_; // Owned value.
    Sink& sink_;           // Borrowed; the caller must outlive this reporter.
public:
    Reporter(Formatter formatter, Sink& sink)
        : formatter_(std::move(formatter)), sink_(sink) {}
    void sample(int value) { sink_.write(formatter_.format(value)); }
};

int main() {
    MemorySink sink;
    Reporter reporter{Formatter{"sample="}, sink};
    reporter.sample(12);
    reporter.sample(14);
    CHECK(sink.messages == std::vector<std::string>({"sample=12", "sample=14"}));
    course::report();
}
