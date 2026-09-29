#include <chrono>
#include <iostream>
#include <algorithm>

int main() {
    using namespace std::chrono_literals;
    const auto initial_budget=100ms,local_elapsed=35ms,reserve=5ms;
    auto forwarded=std::max(0ms,initial_budget-local_elapsed-reserve);
    const auto remote_queue=20ms;
    auto work_budget=std::max(0ms,forwarded-remote_queue);
    std::cout<<"forward_ms="<<forwarded.count()<<" work_ms="<<work_budget.count()<<'\n';
}
