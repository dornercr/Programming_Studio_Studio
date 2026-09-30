#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>

std::string userName(std::string_view record){ if(!record.starts_with("user:")||record.size()==5)throw std::invalid_argument("user record"); return std::string(record.substr(5)); }
