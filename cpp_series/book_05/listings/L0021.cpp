#include "check.hpp"
#include "scale.hpp"
int main() {
    CHECK(harbor::scale(6, 7) == 42);
    CHECK(harbor::scale(-3, 4) == -12);
    CHECK(harbor::scale(0, 10000) == 0);
    std::cout << "PASS\n";
}
