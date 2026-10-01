#include <iostream>
#include <stdexcept>
#include <vector>
void valid(int raw) {
    if (raw < 0 || raw > 100) throw std::invalid_argument("raw");
}
void one_pass(const std::vector<int>& input, int& calls) {
    for (int raw : input) {
        valid(raw);
        ++calls; // Represents calling the conversion step.
    }
}
void two_pass(const std::vector<int>& input, int& calls) {
    for (int raw : input) valid(raw);
    for (int raw : input) {
        (void)raw;
        ++calls;
    }
}
int main() {
    int early = 0;
    int guarded = 0;
    try { one_pass({3, -1}, early); }
    catch (const std::invalid_argument&) {}
    try { two_pass({3, -1}, guarded); }
    catch (const std::invalid_argument&) {}
    std::cout << "one-pass calls=" << early << '\n';
    std::cout << "two-pass calls=" << guarded << '\n';
}
