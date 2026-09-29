#include "store.hpp"
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
namespace harbor {
void save_values(const std::filesystem::path& path, std::span<const int> values) {
    if (values.size() > 1024) throw std::invalid_argument("record count");
    std::ostringstream candidate;
    for (int value : values) {
        if (value < -1000000 || value > 1000000) throw std::invalid_argument("record value");
        candidate << value << '\n';
    }
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) throw std::runtime_error("open output");
    output << candidate.str();
    output.flush();
    if (!output) throw std::runtime_error("write output");
    output.close();
    if (!output) throw std::runtime_error("close output");
}
std::vector<int> load_values(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("open input");
    std::vector<int> result;
    std::string token;
    auto finish = [&] {
        if (token.empty()) return;
        if (result.size() == 1024) throw std::runtime_error("record count");
        int value{};
        const auto [end, error] = std::from_chars(token.data(), token.data()+token.size(), value);
        if (error != std::errc{} || end != token.data()+token.size() || value < -1000000 || value > 1000000)
            throw std::runtime_error("record value");
        result.push_back(value); token.clear();
    };
    char character{};
    while (input.get(character)) {
        if (character == ' ' || character == '\n' || character == '\r' || character == '\t') finish();
        else {
            if (token.size() == 16) throw std::runtime_error("record length");
            token.push_back(character);
        }
    }
    if (!input.eof()) throw std::runtime_error("read input");
    finish();
    return result;
}
}
