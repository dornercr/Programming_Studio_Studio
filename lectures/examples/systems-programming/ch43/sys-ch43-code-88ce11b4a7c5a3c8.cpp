#include <future>
#include <iostream>
#include <string>
int main() {
    auto first = std::async(std::launch::async, [word = std::string("red")] {
        return word.size();
    });
    auto second = std::async(std::launch::async, [word = std::string("blue")] {
        return word.size();
    });
    const auto a = first.get();
    const auto b = second.get();
    std::cout << "first=" << a << " second=" << b << " total=" << a+b << '\n';
}
