#include <chrono>
#include <optional>
#include <iostream>

int main() {
    using namespace std::chrono_literals;
    struct Reply{int value;std::chrono::milliseconds elapsed;};
    auto call=[](bool slow){return Reply{49,slow?200ms:20ms};};
    auto request=[&](bool slow)->std::optional<int>{auto r=call(slow);if(r.elapsed>100ms)return {};return r.value;};
    auto fast=request(false),slow=request(true);
    std::cout<<"fast="<<*fast<<" slow-accepted="<<std::boolalpha<<slow.has_value()<<'\n';
}
