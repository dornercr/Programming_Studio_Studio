#include <iostream>
#include <string_view>
std::string_view choose(std::string_view format, int pages) {
    // Mandatory validation is independent of the selected desk.
    if (pages < 1 || pages > 1000) return "rejected";
    if (format == "label") return "label desk";
    if (format == "report") return "general desk";
    return "unhandled";
}
int main() {
    std::cout << choose("label", 2) << '\n';
    std::cout << choose("report", 6) << '\n';
    std::cout << choose("archive", 1) << '\n';
    std::cout << choose("label", 0) << '\n';
}
