#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string pending, destination;
    int flushes = 0;
    const auto flush = [&] { destination += pending; pending.clear(); ++flushes; };
    pending += "AB";
    pending += "CD";
    if (pending.size() >= 4) flush();
    pending += "E";
    flush();
    std::cout << destination << " flushes=" << flushes << '\n';
    assert(destination == "ABCDE" && flushes == 2);
}
