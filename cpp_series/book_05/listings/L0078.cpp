#include "check.hpp"
#include <array>
#include <cstdint>
#include <span>
std::uint64_t weighted(std::span<const std::uint32_t> values) {
    std::uint64_t sum=0;
    for (auto value:values) sum+=static_cast<std::uint64_t>(value)*3u+1u;
    return sum;
}
std::uint64_t factored(std::span<const std::uint32_t> values) {
    std::uint64_t sum=0;
    for (auto value:values) sum+=value;
    return sum*3u+values.size();
}
int main() {
    const std::array<std::uint32_t,7> values{0,1,2,3,10,100,1000};
    CHECK(weighted(values)==factored(values));
    CHECK(weighted({})==0 && factored({})==0);
    std::cout << "result=" << weighted(values) << "\nPASS\n";
}
