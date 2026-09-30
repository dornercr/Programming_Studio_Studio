#include "check.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>

struct Record { char tag; std::uint32_t count; double value; };
static_assert(std::is_standard_layout_v<Record>);
static_assert(std::is_trivially_copyable_v<Record>);
static_assert(!std::is_trivially_copyable_v<std::string>);

int main() {
    const Record source{'A', 17, 2.5};
    Record destination{};
    std::memcpy(&destination, &source, sizeof(Record));
    CHECK(destination.tag == 'A' && destination.count == 17 && destination.value == 2.5);
    CHECK(sizeof(Record) >= sizeof(char) + sizeof(std::uint32_t) + sizeof(double));
    std::cout << "size=" << sizeof(Record) << " alignment=" << alignof(Record)
              << " offsets=" << offsetof(Record, tag) << ',' << offsetof(Record, count)
              << ',' << offsetof(Record, value) << "\nPASS\n";
}
