#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string a = "running";
    std::string b = "ready";
    a = "waiting";
    b = "running";
    std::cout << "A=" << a << " B=" << b << '\n';
    a = "ready";
    std::cout << "A=" << a << " B=" << b << '\n';
}
