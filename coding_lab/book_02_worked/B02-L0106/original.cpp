#include "report.hpp"
#include <fstream>
#include <iostream>
#include <new>
#include <string_view>

int main(int argc, char* argv[]) {
    try {
        bool grouped = false;
        const char* filename = nullptr;
        for (int index = 1; index < argc; ++index) {
            const std::string_view argument(argv[index]);
            if (argument == "--grouped") {
                grouped = true;
            } else if (argument == "--help") {
                std::cout << "usage: harbor_reports [--grouped] [FILE]\n";
                return std::cout ? 0 : 3;
            } else if (argument.starts_with('-') || filename != nullptr) {
                std::cerr << "usage: harbor_reports [--grouped] [FILE]\n";
                return 2;
            } else {
                filename = argv[index];
            }
        }
        std::ifstream file;
        std::istream* input = &std::cin; // Borrow; neither path owns cin.
        if (filename != nullptr) {
            file.open(filename, std::ios::binary);
            if (!file) {
                std::cerr << "cannot open input\n";
                return 2;
            }
            input = &file;
        }
        harbor::Report report;
        if (const auto error = report.load(*input)) {
            std::cerr << "line " << error->line << ": "
                      << harbor::error_text(error->code) << '\n';
            return 2;
        }
        report.write(std::cout, grouped);
        std::cout.flush();
        return std::cout ? 0 : 3;
    } catch (const std::bad_alloc&) {
        std::cerr << "resource allocation failed\n";
        return 3;
    } catch (const std::exception& error) {
        std::cerr << "operation failed: " << error.what() << '\n';
        return 3;
    }
}
