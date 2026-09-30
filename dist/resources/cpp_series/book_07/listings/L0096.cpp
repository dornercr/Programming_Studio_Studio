#include <iostream>

int main() {
    int last_seen_ms=0;constexpr int timeout=100;
    auto suspected=[&](int now){return now-last_seen_ms>timeout;};
    bool at90=suspected(90),at120=suspected(120);
    last_seen_ms=125;bool after_late=suspected(130);
    std::cout<<std::boolalpha<<"suspect90="<<at90<<" suspect120="<<at120<<" after-heartbeat="<<after_late<<'\n';
}
