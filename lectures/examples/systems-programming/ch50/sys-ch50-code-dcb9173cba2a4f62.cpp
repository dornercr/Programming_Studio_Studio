#include <algorithm>
#include <array>
#include <iostream>
#include <mutex>
#include <thread>
int main() {
    std::array<int,3> values{7,8,9};
    std::mutex first, second, third;
    const auto rotate = [&](bool left) {
        for (int i=0; i<100; ++i) {
            std::scoped_lock lock(first, second, third);
            const auto middle = values.begin() + (left ? 1 : 2);
            std::rotate(values.begin(), middle, values.end());
        }
    };
    std::jthread a(rotate,true), b(rotate,false);
    a.join(); b.join();
    std::cout << "values=" << values[0] << ',' << values[1] << ',' << values[2]
              << " total=" << values[0]+values[1]+values[2] << '\n';
}
