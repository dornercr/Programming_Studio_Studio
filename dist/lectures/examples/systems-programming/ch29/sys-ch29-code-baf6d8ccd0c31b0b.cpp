#include <array>
#include <iostream>
struct Line { bool valid; unsigned tag; };
bool hit(const std::array<Line,2>& set, unsigned requested) {
    for (const auto& line : set)
        if (line.valid && line.tag == requested) return true;
    return false;
}
int main() {
    const std::array<Line,2> set{{{false,0},{true,1}}};
    std::cout << std::boolalpha;
    std::cout << "tag-only-zero=" << (set[0].tag == 0) << '\n';
    std::cout << "valid-zero=" << hit(set,0) << '\n';
    std::cout << "valid-one=" << hit(set,1) << '\n';
}
