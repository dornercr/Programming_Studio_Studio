#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
int main() {
    std::mutex mutex;
    std::condition_variable changed;
    bool ready = false;
    changed.notify_one(); // This does not set ready or store a wakeup token.
    std::unique_lock lock(mutex);
    const bool notification_only = changed.wait_for(
        lock, std::chrono::milliseconds(0), [&] { return ready; });
    ready = true;
    const bool state_ready = changed.wait_for(
        lock, std::chrono::milliseconds(0), [&] { return ready; });
    std::cout << std::boolalpha << "notification only=" << notification_only << '\n';
    std::cout << "state ready=" << state_ready << '\n';
}
