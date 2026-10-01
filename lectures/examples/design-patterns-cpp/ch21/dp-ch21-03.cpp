#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
void validate(const std::vector<int>& input) {
    if (input.empty()) throw std::invalid_argument("empty");
    for (int raw : input)
        if (raw < 0 || raw > 100)
            throw std::invalid_argument("raw");
}
std::string plain(const std::vector<int>& input) {
    validate(input);
    std::ostringstream out;
    for (int raw : input) out << raw << ' ';
    return out.str();
}
std::string doubled(const std::vector<int>& input) {
    validate(input);
    std::ostringstream out;
    for (int raw : input) out << raw * 2 << ' ';
    return out.str() + "scaled";
}
int main() {
    std::cout << '[' << plain({2, 7}) << "]\n";
    std::cout << '[' << doubled({2, 7}) << "]\n";
}
