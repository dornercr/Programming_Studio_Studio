#include <iostream>
#include <stdexcept>
#include <vector>

void direct_append(std::vector<int>& live, const std::vector<int>& batch) {
    for (int id : batch) {
        if (id <= 0) throw std::invalid_argument("bad id");
        live.push_back(id); // Defect: a later error leaves a partial batch.
    }
}
void staged_append(std::vector<int>& live, const std::vector<int>& batch) {
    auto candidate = live;
    for (int id : batch) {
        if (id <= 0) throw std::invalid_argument("bad id");
        candidate.push_back(id);
    }
    live.swap(candidate); // Commit only after all items pass.
}
int main() {
    std::vector<int> direct{10};
    std::vector<int> staged{10};
    const std::vector<int> batch{11, 0};
    try { direct_append(direct, batch); }
    catch (const std::invalid_argument&) {}
    try { staged_append(staged, batch); }
    catch (const std::invalid_argument&) {}
    std::cout << "direct count=" << direct.size() << '\n';
    std::cout << "staged count=" << staged.size() << '\n';
}
