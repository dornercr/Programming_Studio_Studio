#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string backing = "cat";
    auto private_view = backing;
    auto& shared_view = backing;
    private_view[0] = 'b';
    shared_view[2] = 'r';
    std::cout << backing << ' ' << private_view << '\n';
    assert(backing == "car" && private_view == "bat");
}
