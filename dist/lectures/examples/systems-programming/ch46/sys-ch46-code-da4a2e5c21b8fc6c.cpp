#include <atomic>
#include <iostream>
int main() {
    std::atomic<int> value{3};
    int expected = 2;
    const bool first = value.compare_exchange_strong(expected, expected+1);
    std::cout << std::boolalpha;
    std::cout << "first=" << first << " observed=" << expected << '\n';
    const bool second = value.compare_exchange_strong(expected, expected+1);
    std::cout << "second=" << second << " value=" << value.load() << '\n';
}
