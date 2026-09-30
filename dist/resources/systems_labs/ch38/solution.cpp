#include <cassert>
#include <iostream>
#include <string>

bool replace_image(std::string& image, const std::string& candidate) {
    if(candidate.empty()) return false;
    std::string prepared = candidate;
    image.swap(prepared);
    return true;
}

int main() {
    const int pid = 42; std::string image = "shell";
    assert(!replace_image(image,"") && image == "shell");
    assert(replace_image(image,"worker") && image == "worker" && pid == 42);
    std::cout << "pid=" << pid << " image=" << image << '\n';
}
