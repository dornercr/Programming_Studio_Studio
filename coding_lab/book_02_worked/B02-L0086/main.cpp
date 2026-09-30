// Exact original book listing B02-L0110
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

// Original book listing B02-L0086
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>

int main() {
    namespace fs = std::filesystem;
    // The test runner supplies a fresh working directory for every test case.
    const fs::path directory{"chapter20-data"};
    CHECK(fs::create_directory(directory));
    const auto start = std::chrono::steady_clock::now();
    {
        std::ofstream out{directory / "readings.txt"};
        out.exceptions(std::ios::badbit | std::ios::failbit);
        out << "12\n24\n";
        out.close(); // Surface flush/close errors at the explicit boundary.
    }
    {
        std::ifstream in{directory / "readings.txt"};
        int a{}, b{};
        CHECK(static_cast<bool>(in >> a >> b));
        CHECK(a == 12 && b == 24);
    }
    const std::array<unsigned char, 4> bytes{0x12, 0x34, 0x56, 0x78};
    {
        std::ofstream out{directory / "value.bin", std::ios::binary};
        out.exceptions(std::ios::badbit | std::ios::failbit);
        out.write(reinterpret_cast<const char*>(bytes.data()), 4);
        out.close();
    }
    std::array<unsigned char, 4> decoded{};
    {
        std::ifstream in{directory / "value.bin", std::ios::binary};
        in.read(reinterpret_cast<char*>(decoded.data()), 4);
        CHECK(in.gcount() == 4 && decoded == bytes);
    }
    const std::uint32_t value = (std::uint32_t(decoded[0]) << 24)
        | (std::uint32_t(decoded[1]) << 16) | (std::uint32_t(decoded[2]) << 8)
        | std::uint32_t(decoded[3]);
    CHECK(value == 0x12345678U);
    std::mt19937 engine{42};
    std::uniform_int_distribution<int> distribution{1, 6};
    for (int i = 0; i < 100; ++i) {
        const int roll = distribution(engine);
        CHECK(roll >= 1 && roll <= 6);
    }
    CHECK(std::chrono::steady_clock::now() >= start);
    CHECK(fs::remove_all(directory) == 3);
    course::report();
}
