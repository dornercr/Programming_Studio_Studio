#include "check.hpp"
#include "summary.hpp"
#include <array>
#include <stdexcept>
int main() {
    const std::array values{2, 4, 6};
    const auto result = harbor::summarize(values);
    CHECK(result.count == 3 && result.total == 12 && result.mean == 4.0);
    bool rejected = false;
    try { (void)harbor::summarize({}); }
    catch (const std::invalid_argument&) { rejected = true; }
    CHECK(rejected);
    std::cout << "PASS\n";
}
