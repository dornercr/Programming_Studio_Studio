#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>
void add_activity(std::vector<int>& minutes, int duration) {
    if (duration < 1 || duration > 60 || minutes.size() == 3)
        throw std::invalid_argument("activity");
    minutes.push_back(duration);
}
int main() {
    std::vector<int> minutes;
    add_activity(minutes, 60);
    add_activity(minutes, 60);
    const int total = std::accumulate(minutes.begin(), minutes.end(), 0);
    std::cout << "accepted steps=" << minutes.size() << '\n';
    std::cout << "draft total=" << total << '\n';
    // Passing step checks does not authorize publication.
    std::cout << "within final limit=" << std::boolalpha
              << (total <= 90) << '\n';
}
