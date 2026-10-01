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
