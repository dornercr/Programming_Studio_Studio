#include "course_test.hpp"
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
