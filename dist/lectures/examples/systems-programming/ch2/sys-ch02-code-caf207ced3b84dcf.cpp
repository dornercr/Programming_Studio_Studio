#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

std::vector<int> copy_changed(std::vector<int> values, int replacement) {
    if(values.empty()) throw std::invalid_argument("empty");
    values[0] = replacement;
    return values;
}
void change_in_place(std::vector<int>& values, int replacement) {
    if(values.empty()) throw std::invalid_argument("empty");
    values[0] = replacement;
}

int main() {
    std::vector<int> source{2,4};
    const auto copy = copy_changed(source,9);
    assert(source[0] == 2 && copy[0] == 9);
    change_in_place(source,7);
    std::vector<int> empty;
    int rejected = 0;
    try { copy_changed(empty,1); } catch(const std::invalid_argument&) { ++rejected; }
    try { change_in_place(empty,1); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2 && source[0] == 7);
    std::cout << "source=" << source[0] << " copy=" << copy[0] << " rejected=" << rejected << '\n';
}
