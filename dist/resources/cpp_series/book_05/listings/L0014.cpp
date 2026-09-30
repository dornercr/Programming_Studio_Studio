#include "check.hpp"
#include "calibration.hpp"
#include <limits>
#include <stdexcept>
int main() {
    CHECK(harbor::calibrate(4.0, 2.0, -1.0) == 7.0);
    bool rejected = false;
    try { (void)harbor::calibrate(std::numeric_limits<double>::infinity(), 1, 0); }
    catch (const std::invalid_argument&) { rejected = true; }
    CHECK(rejected);
    std::cout << "PASS\n";
}
