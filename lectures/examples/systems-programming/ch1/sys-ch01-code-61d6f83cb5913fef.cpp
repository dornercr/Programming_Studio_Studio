#include <cassert>
#include <iostream>
#include <string>

int sum(int first, int second) { return first + second; }
std::string format(const std::string& label, int value) {
    return label + "=" + std::to_string(value);
}

int main() {
    assert(sum(7,5) == 12 && sum(-2,5) == 3);
    assert(format("sum",3) == "sum=3");
    std::cout << format("total",sum(7,5)) << '\n';
    std::cout << format("sum",sum(-2,5)) << '\n';
}
