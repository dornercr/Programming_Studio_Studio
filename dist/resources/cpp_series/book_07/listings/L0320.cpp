#pragma once
#include <array>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
namespace harbor {
// Single-owner demonstration. Real multi-thread aggregation needs synchronization.
class Histogram {
    std::array<double,4> bounds_{0.005,0.02,0.1,1.0};
    std::array<std::uint64_t,5> counts_{};
    double sum_=0;
public:
    void observe(double seconds) {
        if (!(seconds>=0) || !std::isfinite(seconds)) throw std::invalid_argument("latency");
        sum_+=seconds;
        for (std::size_t i=0;i<bounds_.size();++i) if (seconds<=bounds_[i]) ++counts_[i];
        ++counts_.back();
    }
    std::string text() const {
        std::ostringstream out;
        for (std::size_t i=0;i<bounds_.size();++i)
            out<<"harbor_request_seconds_bucket{le=\""<<bounds_[i]<<"\"} "<<counts_[i]<<'\n';
        out<<"harbor_request_seconds_bucket{le=\"+Inf\"} "<<counts_.back()<<'\n';
        out<<"harbor_request_seconds_count "<<counts_.back()<<'\n';
        out<<"harbor_request_seconds_sum "<<sum_<<'\n';
        return out.str();
    }
};
}
