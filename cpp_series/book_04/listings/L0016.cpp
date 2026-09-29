#include "check.hpp"
#include <fstream>
#include <memory>
#include <string>

int initialized_global = 17;
int zero_initialized_global;
const int read_only_candidate = 23;

int main() {
    int automatic = 5;
    auto dynamic = std::make_unique<int>(9);
    CHECK(zero_initialized_global == 0);
    std::cout << "global=" << static_cast<const void*>(&initialized_global)
              << " zero_global=" << static_cast<const void*>(&zero_initialized_global)
              << " const_object=" << static_cast<const void*>(&read_only_candidate)
              << " automatic=" << static_cast<const void*>(&automatic)
              << " dynamic=" << static_cast<const void*>(dynamic.get()) << '\n';
    std::ifstream maps{"/proc/self/maps"};
    CHECK(maps.is_open());
    std::string line;
    std::size_t lines = 0;
    while (std::getline(maps, line)) {
        if (lines < 3) std::cout << line << '\n';
        ++lines;
    }
    CHECK(maps.eof() && !maps.bad() && lines > 0);
    std::cout << "mapping_count=" << lines << "\nPASS\n";
}
