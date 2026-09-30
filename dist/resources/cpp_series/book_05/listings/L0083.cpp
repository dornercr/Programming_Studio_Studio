#include "check.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
std::uint64_t serial_sum(std::span<const std::uint32_t> values) {
    std::uint64_t sum=0; for (auto value:values) sum+=value; return sum;
}
std::uint64_t independent_sum(std::span<const std::uint32_t> values) {
    std::array<std::uint64_t,8> lanes{};
    std::size_t i=0;
    for (;i+8<=values.size();i+=8)
        for (std::size_t lane=0;lane<8;++lane) lanes[lane]+=values[i+lane];
    std::uint64_t sum=0; for (auto lane:lanes) sum+=lane;
    for (;i<values.size();++i) sum+=values[i];
    return sum;
}
int main() {
    for (std::size_t size=0;size<130;++size) {
        std::vector<std::uint32_t> values(size);
        for (std::size_t i=0;i<size;++i) values[i]=static_cast<std::uint32_t>(i*7);
        CHECK(serial_sum(values)==independent_sum(values));
    }
    std::cout << "tested_lengths=130\nPASS\n";
}
