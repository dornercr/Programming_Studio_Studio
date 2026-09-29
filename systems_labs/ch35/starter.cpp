// LAB: Flush the final partial buffer
// Combine characters into batches of four, then forward any remaining suffix at the end. Do not count an empty final flush as a data transfer.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
