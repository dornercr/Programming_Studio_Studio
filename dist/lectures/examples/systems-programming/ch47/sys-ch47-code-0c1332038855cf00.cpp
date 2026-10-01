// LAB: Guard a conservation rule across real workers
// Two accounts begin with 100 each. Two workers make 100 opposite-direction transfers of one unit. Protect each complete transfer with the same mutex.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <mutex>

int main() {
    int first = 60, second = 40;
    std::mutex guard;
    {
        std::lock_guard<std::mutex> lock(guard);
        assert(first >= 10);
        first -= 10;
        second += 10;
        assert(first + second == 100);
    }
    std::cout << first << ' ' << second << '\n';
}
