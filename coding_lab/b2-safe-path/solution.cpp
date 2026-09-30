#include <iostream>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

std::optional<std::filesystem::path> reportPath(const std::filesystem::path& root,std::string_view filename){if(filename.empty()||filename.find('/')!=std::string_view::npos||filename.find('\\')!=std::string_view::npos)return std::nullopt;std::filesystem::path leaf{std::string(filename)};if(leaf=="."||leaf==".."||leaf.filename()!=leaf||leaf.extension()!=".txt")return std::nullopt;return root/leaf;}
