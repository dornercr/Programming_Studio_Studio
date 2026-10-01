#include <iostream>
#include <stdexcept>
#include <string>
std::string text_job(const std::string& label) {
    if (label.empty() || label.size() > 20)
        throw std::invalid_argument("label");
    return "stored text:" + label;
}
std::string copied_bracket_job(const std::string& label) {
    // BUG: this copied branch lost the length rule.
    if (label.empty()) throw std::invalid_argument("label");
    return "stored [" + label + "]";
}
int main() {
    const std::string label(21, 'x');
    try { text_job(label); }
    catch (const std::invalid_argument&) {
        std::cout << "text: rejected\n";
    }
    const auto receipt = copied_bracket_job(label);
    std::cout << "bracket: accepted " << label.size()
              << " bytes\n";
    std::cout << "receipt bytes=" << receipt.size() << '\n';
}
