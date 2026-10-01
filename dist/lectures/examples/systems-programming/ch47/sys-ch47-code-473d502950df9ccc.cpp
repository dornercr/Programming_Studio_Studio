#include <iostream>
#include <mutex>
#include <stdexcept>
int main() {
    std::mutex mutex;
    int value = 7;
    bool rejected = false;
    try {
        std::lock_guard lock(mutex);
        const int candidate = -1;
        if (candidate < 0) throw std::invalid_argument("negative");
        value = candidate;
    } catch (const std::invalid_argument&) { rejected = true; }
    {
        std::lock_guard lock(mutex); // Prior guard released on the exception.
        ++value;
    }
    std::cout << std::boolalpha << "rejected=" << rejected
              << " value=" << value << '\n';
}
