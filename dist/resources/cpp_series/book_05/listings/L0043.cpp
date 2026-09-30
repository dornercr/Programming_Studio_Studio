#ifndef HARBOR_STORE_HPP
#define HARBOR_STORE_HPP
#include <filesystem>
#include <span>
#include <vector>
namespace harbor {
void save_values(const std::filesystem::path&, std::span<const int>);
std::vector<int> load_values(const std::filesystem::path&);
}
#endif
