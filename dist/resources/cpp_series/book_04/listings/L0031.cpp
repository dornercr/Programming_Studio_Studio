#include "check.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

struct Compact { std::uint64_t value{}; };
struct alignas(64) Separated { std::uint64_t value{}; };
static_assert(alignof(Separated) >= 64);
static_assert(sizeof(Separated) % alignof(Separated) == 0);

int main() {
    std::vector<Separated> counters(4);
    for (std::size_t i = 0; i < counters.size(); ++i) {
        const auto address = reinterpret_cast<std::uintptr_t>(&counters[i]);
        CHECK(address % alignof(Separated) == 0);
        counters[i].value = i + 1;
    }
    CHECK(counters[3].value == 4);
    std::cout << "compact_size=" << sizeof(Compact)
              << " separated_size=" << sizeof(Separated)
              << " requested_alignment=" << alignof(Separated) << "\nPASS\n";
}
