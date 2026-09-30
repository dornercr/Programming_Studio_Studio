#include "check.hpp"
#include "store.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
int main() {
    // The supplied fixture runner gives this test a fresh working directory.
    const auto path = std::filesystem::path{"measurements.fixture"};
    const std::vector values{3, -2, 8};
    harbor::save_values(path, values);
    CHECK(harbor::load_values(path) == values);
    std::ifstream input(path, std::ios::binary);
    const std::string bytes((std::istreambuf_iterator<char>(input)), {});
    CHECK(bytes == "3\n-2\n8\n");
    input.close();
    { std::ofstream bad(path, std::ios::trunc); bad << "3\n2x\n"; }
    bool rejected = false;
    try { (void)harbor::load_values(path); }
    catch (const std::runtime_error&) { rejected = true; }
    CHECK(rejected);
    CHECK(std::filesystem::remove(path));
    std::cout << "PASS\n";
}
