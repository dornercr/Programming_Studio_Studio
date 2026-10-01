#include <iostream>
#include <vector>

struct Record {
    double celsius;
    bool alert_at_intake;
};
int main() {
    double limit = 80.0;
    std::vector<Record> history{{80.0, 80.0 >= limit}};
    limit = 100.0; // New policy is for future intake.
    int stored = 0;
    int recomputed = 0;
    for (const auto& row : history) {
        stored += row.alert_at_intake ? 1 : 0;
        recomputed += row.celsius >= limit ? 1 : 0;
    }
    std::cout << "stored alerts=" << stored << '\n';
    std::cout << "recomputed alerts=" << recomputed << '\n';
    std::cout << "future alert=" << (90.0 >= limit) << '\n';
}
