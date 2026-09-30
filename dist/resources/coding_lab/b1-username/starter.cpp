#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>

std::string userName(std::string_view record){ return std::string(record.substr(5)); // BUG: no prefix check
}
