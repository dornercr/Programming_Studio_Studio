#include <cassert>
#include <iostream>
#include <string>

int main() {
    const int pid = 42;
    std::string image = "shell";
    image = "worker";
    std::cout << "pid=" << pid << " image=" << image << '\n';
    assert(pid == 42 && image == "worker");
}
