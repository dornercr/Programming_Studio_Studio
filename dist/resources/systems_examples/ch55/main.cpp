#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    const std::vector<int> values{2, 4, 6, 8};
    std::vector<int> prefix(values.size() + 1, 0);
    for (std::size_t i = 0; i < values.size(); ++i) prefix[i + 1] = prefix[i] + values[i];
    const int answer = prefix[4] - prefix[1];
    std::cout << answer << '\n';
    assert(answer == 18);
}
