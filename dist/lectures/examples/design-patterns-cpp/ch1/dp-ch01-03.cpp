#include <iostream>
#include <stdexcept>
#include <string>
std::string archive_text(const std::string& label) {
    // One format needs no class family.
    if (label.empty() || label.size() > 20)
        throw std::invalid_argument("label");
    return "stored text:" + label;
}
int main() {
    std::cout << archive_text("notes") << '\n';
    try { archive_text(""); }
    catch (const std::invalid_argument&) {
        std::cout << "empty rejected\n";
    }
}
