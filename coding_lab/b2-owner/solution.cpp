#include <iostream>
#include <memory>
#include <utility>

std::unique_ptr<int> makeReading(int value) {
    return std::make_unique<int>(value);
}
