// LAB: Partition an uneven range safely
// Sum a vector with two asynchronous workers, splitting at size/2. Keep the input alive until both results are obtained and support empty and odd-length inputs.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <array>
#include <cassert>
#include <iostream>
#include <thread>

int main() {
    const std::array<int, 4> input{1, 2, 3, 4};
    std::array<int, 2> partial{0, 0};
    std::thread a([&] { partial[0] = input[0] + input[1]; });
    std::thread b([&] { partial[1] = input[2] + input[3]; });
    a.join();
    b.join();
    std::cout << partial[0] + partial[1] << '\n';
    assert(partial[0] + partial[1] == 10);
}
