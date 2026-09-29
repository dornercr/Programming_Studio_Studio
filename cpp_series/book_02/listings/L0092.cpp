#include "report.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc > 2) { std::cerr << "usage: report [input-file]\n"; return 1; }
    std::ifstream file;
    std::istream* input = &std::cin; // Borrow, never delete.
    if (argc == 2) {
        file.open(argv[1]);
        if (!file) { std::cerr << "error: cannot open input\n"; return 1; }
        input = &file;
    }
    harbor::Report report;
    if (const auto error = report.load(*input)) {
        std::cerr << "error line " << error->line << ": " << error->message << '\n';
        return 1;
    }
    report.write(std::cout);
    if (!std::cout) { std::cerr << "error: output failed\n"; return 1; }
}
