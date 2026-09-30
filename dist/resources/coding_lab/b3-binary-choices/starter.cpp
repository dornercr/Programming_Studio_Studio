#include <iostream>
#include <vector>
#include <string>
#include <cstddef>
#include <stdexcept>

std::vector<std::string> binaries(std::size_t n){if(n>10)throw std::invalid_argument("too long");return {std::string(n,'0')};}
