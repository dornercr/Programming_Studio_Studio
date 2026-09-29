#include "check.hpp"
#include <atomic>
#include <thread>

int main() {
    std::atomic<unsigned> count{0};
    std::jthread a([&] { for (unsigned i = 0; i < 10000; ++i) count.fetch_add(1, std::memory_order_relaxed); });
    std::jthread b([&] { for (unsigned i = 0; i < 10000; ++i) count.fetch_add(1, std::memory_order_relaxed); });
    a.join(); b.join();
    CHECK(count.load(std::memory_order_relaxed) == 20000);

    int payload = 0;
    int observed = 0;
    std::atomic<bool> ready{false};
    // Start the finite producer first. If consumer construction fails,
    // producer cleanup can still finish without waiting for a missing peer.
    std::jthread producer([&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
        ready.notify_one();
    });
    std::jthread consumer([&] {
        ready.wait(false, std::memory_order_acquire);
        observed = payload;
    });
    producer.join(); consumer.join();
    CHECK(observed == 42);
    std::cout << "unsigned_atomic_lock_free=" << count.is_lock_free() << "\nPASS\n";
}
