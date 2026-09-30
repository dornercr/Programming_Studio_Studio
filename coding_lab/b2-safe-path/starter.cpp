#include <iostream>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

std::optional<std::filesystem::path> reportPath(const std::filesystem::path& root,std::string_view filename){return root/std::string(filename);}
