#include <cassert>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <future>

int handoff() {
    std::mutex mutex;
    std::condition_variable changed;
    bool ready = false; int payload = 0;
    auto producer = std::async(std::launch::async,[&] {
        { std::lock_guard<std::mutex> lock(mutex); payload = 42; ready = true; }
        changed.notify_one();
    });
    int observed;
    {
        std::unique_lock<std::mutex> lock(mutex);
        changed.wait(lock,[&] { return ready; });
        observed = payload;
    }
    producer.get();
    return observed;
}

int main() {
    assert(handoff() == 42);
    std::cout << "payload=" << handoff() << '\n';
}
