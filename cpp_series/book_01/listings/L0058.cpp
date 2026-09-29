#include <iostream>

int groups_needed(int items, int capacity = 5) {
    if (items < 0 || capacity <= 0) return -1;
    return items / capacity + (items % capacity != 0 ? 1 : 0);
}

void record_success(int& successful_reports) {
    ++successful_reports;
}

int main() {
    int reports{};
    const int groups = groups_needed(23);
    if (groups >= 0) record_success(reports);
    if (groups != 5 || reports != 1) return 1;
    if (groups_needed(0) != 0) return 2;
    if (groups_needed(4, 0) != -1) return 3;
    std::cout << "groups=" << groups << " reports=" << reports << '\n';
    std::cout << "PASS\n";
}
