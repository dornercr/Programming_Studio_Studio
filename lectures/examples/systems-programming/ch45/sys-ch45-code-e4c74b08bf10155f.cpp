#include <iostream>
int main() {
    // Serialized model of two stale decisions; no threads or data race.
    int stock = 1;
    const int first_snapshot = stock;
    const int second_snapshot = stock;
    bool first_grant = false, second_grant = false;
    if (first_snapshot > 0) { stock = first_snapshot-1; first_grant = true; }
    if (second_snapshot > 0) { stock = second_snapshot-1; second_grant = true; }
    const int grants = int(first_grant) + int(second_grant);
    std::cout << "remaining=" << stock << " grants=" << grants
              << " accounted=" << stock+grants << '\n';
}
