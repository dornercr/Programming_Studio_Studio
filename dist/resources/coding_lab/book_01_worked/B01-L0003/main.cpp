#include <algorithm>
#include <iostream>
#include <vector>
struct Reading { int id; double value; };
int main() {
    std::vector<Reading> r{{3, 18.2}, {1, 20.1}, {2, 19.4}};
    std::sort(r.begin(), r.end(), [](const Reading& a, const Reading& b){ return a.id < b.id; });
    for (const auto& x : r) std::cout << x.id << ':' << x.value << '\n';
}
