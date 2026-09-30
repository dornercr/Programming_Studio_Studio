#include "check.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <span>
#include <string_view>
#include <vector>
std::size_t scan_hits(std::span<const int> values, std::span<const int> queries) {
    std::size_t hits=0;
    for (int query:queries) if (std::find(values.begin(),values.end(),query)!=values.end()) ++hits;
    return hits;
}
std::size_t binary_hits(std::span<const int> values, std::span<const int> queries) {
    std::size_t hits=0;
    for (int query:queries) if (std::binary_search(values.begin(),values.end(),query)) ++hits;
    return hits;
}
int main(int argc,char** argv) {
    const bool profile=argc==2 && std::string_view(argv[1])=="--profile";
    if (argc>1 && !profile) return 2;
    std::vector<int> values(4096); std::iota(values.begin(),values.end(),0);
    std::vector<int> queries(1024);
    std::uint32_t state=17;
    for (int& query:queries) { state=state*1664525u+1013904223u; query=static_cast<int>(state%8192u); }
    const auto expected=scan_hits(values,queries);
    CHECK(binary_hits(values,queries)==expected);
    std::size_t checksum=0;
    const int rounds=profile?200:2;
    for (int round=0;round<rounds;++round) {
        queries[0]=round%8192;
        const auto a=scan_hits(values,queries), b=binary_hits(values,queries);
        CHECK(a==b); checksum+=a+b;
    }
    std::cout << "rounds=" << rounds << " checksum=" << checksum << "\nPASS\n";
}
