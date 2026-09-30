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

// Original book listing B02-L0036
#include <string_view>
#include <vector>

class Device {
    std::vector<std::string_view>& log_;
protected:
    int scale_;
public:
    Device(std::vector<std::string_view>& log, int scale) : log_(log), scale_(scale) {
        log_.push_back("base constructed");
    }
    ~Device() { log_.push_back("base destroyed"); }
    // No deletion through Device*: this is a non-polymorphic teaching base.
};
class Sensor : public Device {
    std::vector<std::string_view>& log_;
public:
    Sensor(std::vector<std::string_view>& log, int scale) : Device(log, scale), log_(log) {
        log_.push_back("derived constructed");
    }
    ~Sensor() { log_.push_back("derived destroyed"); }
    int calibrated(int raw) const { return raw * scale_; }
};

int main() {
    std::vector<std::string_view> log;
    log.reserve(4); // Avoid allocating while recording these destructor events.
    {
        Sensor sensor{log, 2};
        CHECK(sensor.calibrated(8) == 16);
    }
    CHECK(log.size() == 4);
    CHECK(log[0] == "base constructed" && log[1] == "derived constructed");
    CHECK(log[2] == "derived destroyed" && log[3] == "base destroyed");
    course::report();
}
