#include <array>
#include <iostream>
#include <map>
int main() {
    const std::array<int, 6> data{2, 4, 2, 7, 2, 4};
    const std::array<int, 3> queries{2, 4, 7};
    int comparisons = 0, direct = 0;
    for (int key : queries) for (int value : data) {
        ++comparisons;
        direct += key == value;
    }
    std::map<int, int> counts;
    for (int value : data) ++counts[value];
    int indexed = 0;
    for (int key : queries) indexed += counts.at(key);
    if (indexed != direct) return 1;
    std::cout << "direct comparisons=" << comparisons << '\n';
    std::cout << "index visits=" << data.size() << " queries=" << queries.size()
              << " matches=" << indexed << '\n';
}
