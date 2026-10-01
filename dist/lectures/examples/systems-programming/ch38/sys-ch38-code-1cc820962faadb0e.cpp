// LAB: Keep failed replacement from destroying the current image
// Model exec as a prepared replacement. Validate the new program name before committing it; preserve process identity on success and the old image on failure.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
