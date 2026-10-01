#include <iostream>
int main() {
    constexpr unsigned capacity = 128, line_bytes = 16, ways = 2;
    constexpr unsigned slots = capacity/line_bytes;
    constexpr unsigned sets = slots/ways;
    constexpr unsigned address = 112;
    const unsigned block = address/line_bytes;
    std::cout << "slots=" << slots << " sets=" << sets << " ways=" << ways << '\n';
    std::cout << "block=" << block << " set=" << block%sets << " tag=" << block/sets << '\n';
}
