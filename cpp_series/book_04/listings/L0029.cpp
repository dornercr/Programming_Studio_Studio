#include <cstddef>
#include <iostream>

struct alignas(64) CounterSlot {
    long long value{};
};

int main() {
    CounterSlot slots[2];
    auto distance = reinterpret_cast<char*>(&slots[1]) - reinterpret_cast<char*>(&slots[0]);
    std::cout << "slot bytes=" << sizeof(CounterSlot) << " distance=" << distance << "\n";
}
