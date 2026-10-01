#include <iostream>
#include <numeric>
#include <string>
#include <vector>
struct Draft {
    std::string title;
    std::vector<int> minutes;
};
bool valid(const Draft& draft) {
    if (draft.title.empty() || draft.minutes.empty() ||
        draft.minutes.size() > 3) return false;
    for (int duration : draft.minutes)
        if (duration < 1 || duration > 60) return false;
    return std::accumulate(draft.minutes.begin(),
                           draft.minutes.end(), 0) <= 90;
}
int main() {
    Draft draft{"Review", {20, 30}};
    // This caller remembers the validation boundary.
    if (valid(draft)) {
        std::cout << draft.title << ": "
                  << std::accumulate(draft.minutes.begin(),
                                     draft.minutes.end(), 0)
                  << " minutes\n";
    }
    draft.title.clear();
    std::cout << "after edit: valid=" << std::boolalpha
              << valid(draft) << '\n';
}
