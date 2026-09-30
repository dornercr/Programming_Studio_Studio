#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <future>

int publish(int value) {
    int payload = 0;
    std::atomic<bool> ready{false};
    auto producer = std::async(std::launch::async,[&] {
        payload = value;
        ready.store(true,std::memory_order_release);
    });
    while(!ready.load(std::memory_order_acquire)) std::this_thread::yield();
    const int observed = payload;
    producer.get();
    return observed;
}

int main() {
    assert(publish(42) == 42 && publish(-7) == -7);
    std::cout << "first=" << publish(42) << " second=" << publish(-7) << '\n';
}
