#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

std::unique_ptr<int> makeReading(int value){ return std::make_unique<int>(0); // BUG: loses value and validation
}
