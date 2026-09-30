#include "metrics.hpp"
#include <algorithm>
#include <array>
#include <charconv>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
namespace harbor {
namespace {
constexpr std::size_t maximum_records=100000;
Record parse_line(std::string_view line) {
    if (!line.empty() && line.back()=='\r') line.remove_suffix(1);
    const auto comma=line.find(',');
    if (comma==std::string_view::npos || line.find(',',comma+1)!=std::string_view::npos)
        throw std::runtime_error("expected tick,value");
    Record result{};
    auto tick=line.substr(0,comma), value=line.substr(comma+1);
    const auto [a,ae]=std::from_chars(tick.data(),tick.data()+tick.size(),result.tick);
    const auto [b,be]=std::from_chars(value.data(),value.data()+value.size(),result.value);
    if (ae!=std::errc{} || a!=tick.data()+tick.size() || be!=std::errc{} || b!=value.data()+value.size())
        throw std::runtime_error("invalid numeric field");
    return result;
}
void validate(std::span<const Record> records) {
    if (records.empty() || records.size()>maximum_records) throw std::runtime_error("record count");
    std::uint64_t previous=0;
    for (const auto& record:records) {
        if (record.tick>1000000000000ULL || record.tick<previous)
            throw std::runtime_error("timestamp order or range");
        if (record.value < -1000000 || record.value > 1000000)
            throw std::runtime_error("value range");
        previous=record.tick;
    }
}
}
std::vector<Record> read_records(std::istream& input) {
    std::vector<Record> records;
    std::string line;
    auto append=[&] {
        if (records.size()==maximum_records) throw std::runtime_error("record count");
        records.push_back(parse_line(line)); line.clear();
    };
    char character{};
    while (input.get(character)) {
        if (character=='\n') append();
        else {
            if (line.size()==64) throw std::runtime_error("line length");
            line.push_back(character);
        }
    }
    if (!input.eof()) throw std::runtime_error("input read failure");
    if (!line.empty()) append();
    validate(records);
    return records;
}
Summary analyze(std::span<const Record> records,bool four_lanes) {
    validate(records);
    long long total=0;
    if (four_lanes) {
        std::array<long long,4> partial{};
        std::size_t i=0;
        for (;i+4<=records.size();i+=4)
            for (std::size_t lane=0;lane<4;++lane) partial[lane]+=records[i+lane].value;
        for (auto value:partial) total+=value;
        for (;i<records.size();++i) total+=records[i].value;
    } else {
        for (const auto& record:records) total+=record.value;
    }
    int minimum=records.front().value, maximum=minimum;
    for (const auto& record:records) {
        minimum=std::min(minimum,record.value); maximum=std::max(maximum,record.value);
    }
    return {records.size(),total,minimum,maximum,static_cast<double>(total)/static_cast<double>(records.size())};
}
void write_report(std::ostream& output,const Summary& summary) {
    output << "count=" << summary.count << " total=" << summary.total
           << " min=" << summary.minimum << " max=" << summary.maximum
           << " mean=" << std::fixed << std::setprecision(3) << summary.mean << '\n';
    if (!output) throw std::runtime_error("report write failure");
}
}
