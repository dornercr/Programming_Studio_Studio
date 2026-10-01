#include <algorithm>
#include <iostream>

enum class Choice { mean, highest };

int dashboard(Choice choice, int a, int b) {
    if (choice == Choice::highest) return std::max(a, b);
    return (a + b) / 2;
}

int stale_report(Choice, int a, int b) {
    // Deliberate defect: this caller ignores the selected rule.
    return (a + b) / 2;
}

int main() {
    const auto choice = Choice::highest;
    std::cout << "dashboard=" << dashboard(choice, 1, 9) << '\n';
    std::cout << "stale report=" << stale_report(choice, 1, 9) << '\n';
}
