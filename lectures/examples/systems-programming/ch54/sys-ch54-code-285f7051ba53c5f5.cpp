#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

struct Stats { double mean; double p95; double per_second; };
Stats summarize(std::vector<double> times) {
    if(times.empty()) throw std::invalid_argument("empty sample");
    double total = 0;
    for(double time : times) {
        if(!std::isfinite(time) || time < 0) throw std::invalid_argument("duration");
        total += time;
    }
    if(!std::isfinite(total) || total <= 0) throw std::invalid_argument("total time");
    std::sort(times.begin(),times.end());
    const auto rank = static_cast<std::size_t>(std::ceil(.95*times.size()));
    return {total/times.size(),times[rank-1],times.size()/(total/1000)};
}

int main() {
    const auto result = summarize({2,8,5});
    assert(result.mean == 5 && result.p95 == 8 && result.per_second == 200);
    assert(summarize({10}).p95 == 10 && summarize({10}).per_second == 100);
    int rejected = 0;
    for(const auto& values : std::vector<std::vector<double>>{{},{0,0},{-1,2}}) {
        try { summarize(values); } catch(const std::invalid_argument&) { ++rejected; }
    }
    assert(rejected == 3);
    std::cout << "mean=" << result.mean << " p95=" << result.p95 << " rate=" << result.per_second << "/s\n";
}
