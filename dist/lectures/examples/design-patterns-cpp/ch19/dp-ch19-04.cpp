#include <iostream>
enum class Mode { idle, active, done };
bool shortcut_reset(Mode& mode) {
    mode = Mode::idle; // Wrong: accepts reset from every mode.
    return true;
}
bool contract_reset(Mode& mode) {
    if (mode != Mode::done) return false;
    mode = Mode::idle;
    return true;
}
int main() {
    Mode wrong = Mode::active;
    Mode right = Mode::active;
    std::cout << std::boolalpha;
    std::cout << "shortcut accepted: " << shortcut_reset(wrong) << '\n';
    std::cout << "contract accepted: " << contract_reset(right) << '\n';
    std::cout << "contract still active: " << (right == Mode::active) << '\n';
}
