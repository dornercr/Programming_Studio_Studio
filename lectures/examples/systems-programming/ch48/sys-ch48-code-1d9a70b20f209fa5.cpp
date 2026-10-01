#include <array>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
int main() {
    std::mutex mutex;
    std::condition_variable changed;
    bool closed = false;
    std::array<int,2> finished{};
    const auto wait_for_close = [&](int id) {
        std::unique_lock lock(mutex);
        changed.wait(lock, [&] { return closed; });
        finished[id] = 1;
    };
    std::jthread a(wait_for_close,0), b(wait_for_close,1);
    { std::lock_guard lock(mutex); closed = true; }
    changed.notify_all();
    a.join(); b.join();
    std::cout << "finished=" << finished[0]+finished[1] << '\n';
}
