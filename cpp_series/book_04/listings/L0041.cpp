// INTENTIONALLY INVALID. Not part of the default build or passing test suite.
// Compile with AddressSanitizer in an isolated development process.
#include <memory>
int main(int argc, char**) {
    auto data = std::make_unique<int[]>(4);
    volatile int index = argc + 3; // With no extra arguments, index is 4.
    return data[index];           // Deliberate heap out-of-bounds read.
}
