#include "check.hpp"
#include "vendor/bounded.hpp"
#include <string_view>
int main() {
    CHECK(harbor_vendor::bounded(-2, 0, 10) == 0);
    CHECK(harbor_vendor::bounded(5, 0, 10) == 5);
    CHECK(harbor_vendor::bounded(30, 0, 10) == 10);
    CHECK(std::string_view(harbor_vendor::version) == "1.0.0-teaching");
    std::cout << "PASS\n";
}
