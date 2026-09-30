// INTENTIONALLY INVALID. Excluded from the normal build and test suite.
// Run only as a separately instrumented negative experiment.
#include <cstdlib>
#include <iostream>
int main(int argc, char** argv) {
    const int index = argc > 1 ? std::atoi(argv[1]) : 8;
    int* values = new int[8]{};
    std::cout << values[index] << '\n'; // index 8 is deliberately out of bounds.
    delete[] values;
}
