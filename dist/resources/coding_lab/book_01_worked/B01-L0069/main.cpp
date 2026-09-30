#include <iostream>
#include <iterator>

int main() {
    const int frame[]{2, 4, 6, 8};
    int total{};
    for (std::size_t i = 0; i < std::size(frame); ++i) {
        total += frame[i];
    }
    const int grid[2][3]{{1, 2, 3}, {4, 5, 6}};
    int matrix_total{};
    for (const auto& row : grid) {
        for (int value : row) matrix_total += value;
    }
    if (total != 20 || matrix_total != 21) return 1;
    std::cout << "frame=" << total << " matrix=" << matrix_total << '\n';
    std::cout << "PASS\n";
}
