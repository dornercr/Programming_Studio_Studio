#include <array>
#include <iostream>
int main() {
    std::array<int,12> values{};
    for (int i=0; i<12; ++i) values[i] = i;
    int rows = 0, columns = 0;
    for (int row=0; row<3; ++row)
        for (int col=0; col<4; ++col) rows += values[row*4+col];
    for (int col=0; col<4; ++col)
        for (int row=0; row<3; ++row) columns += values[row*4+col];
    std::cout << "rows=" << rows << " columns=" << columns << '\n';
    std::cout << "same=" << std::boolalpha << (rows == columns) << '\n';
}
