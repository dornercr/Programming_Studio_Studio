#ifndef HARBOR_METRICS_HPP
#define HARBOR_METRICS_HPP
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <span>
#include <vector>
namespace harbor {
inline constexpr const char* version="0.1.0";
struct Record { std::uint64_t tick; int value; };
struct Summary { std::size_t count; long long total; int minimum,maximum; double mean; };
std::vector<Record> read_records(std::istream& input);
Summary analyze(std::span<const Record> records,bool four_lanes=false);
void write_report(std::ostream& output,const Summary& summary);
}
#endif
