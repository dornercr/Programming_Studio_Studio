#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono_literals;
    const auto started=std::chrono::steady_clock::time_point{100ms};
    const auto now=std::chrono::steady_clock::time_point{135ms};
    auto remaining=100ms-std::chrono::duration_cast<std::chrono::milliseconds>(now-started);
    const std::chrono::seconds expires_epoch{2000},wall_now_epoch{2001};
    std::cout<<"local-budget-ms="<<remaining.count()<<" persisted-expired="<<std::boolalpha<<(wall_now_epoch>=expires_epoch)<<'\n';
}
