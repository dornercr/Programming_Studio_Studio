#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

std::unique_ptr<int> makeReading(int value){ if(value<0)throw std::invalid_argument("negative reading"); return std::make_unique<int>(value); }
