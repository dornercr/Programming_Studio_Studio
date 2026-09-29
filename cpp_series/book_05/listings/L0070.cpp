#include "check.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <span>
#include <vector>
long long scalar(std::span<const int> values) {
    long long sum = 0; for (int value : values) sum += value; return sum;
}
long long lanes(std::span<const int> values) {
    long long a=0, b=0, c=0, d=0;
    std::size_t i=0;
    for (; i+4 <= values.size(); i+=4) {
        a+=values[i]; b+=values[i+1]; c+=values[i+2]; d+=values[i+3];
    }
    long long sum=a+b+c+d;
    for (; i<values.size(); ++i) sum+=values[i];
    return sum;
}
struct Measurement { double milliseconds; long long checksum; };
template<class Function>
Measurement measure(std::vector<int> values, Function function) {
    long long checksum=0;
    const auto start=std::chrono::steady_clock::now();
    for (int repetition=0; repetition<64; ++repetition) {
        values[0]=repetition;
        checksum+=function(values);
    }
    const auto elapsed=std::chrono::steady_clock::now()-start;
    return {std::chrono::duration<double,std::milli>(elapsed).count(),checksum};
}
int main() {
    std::vector<int> values(8195);
    for (std::size_t i=0;i<values.size();++i) values[i]=static_cast<int>(i%97);
    CHECK(scalar(values)==lanes(values));
    std::array<double,7> first{}, second{};
    long long checksum=0;
    for (std::size_t round=0;round<first.size();++round) {
        const auto a=measure(values,scalar), b=measure(values,lanes);
        CHECK(a.checksum==b.checksum);
        first[round]=a.milliseconds; second[round]=b.milliseconds;
        checksum=a.checksum;
        std::cout << "round=" << round << " scalar_ms=" << first[round]
                  << " lanes_ms=" << second[round] << '\n';
    }
    std::sort(first.begin(),first.end()); std::sort(second.begin(),second.end());
    std::cout << "median_scalar_ms=" << first[3] << " median_lanes_ms=" << second[3]
              << " checksum=" << checksum << "\nPASS\n";
}
