#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

bool append_bounded(std::vector<int>& values, int value, std::size_t maximum) {
    if(values.size() >= maximum) return false;
    values.push_back(value);
    return true;
}

int main() {
    std::vector<int> values{4,6};
    assert(append_bounded(values,8,3));
    const auto before = values;
    assert(!append_bounded(values,10,3) && values == before);
    std::vector<int> empty;
    assert(!append_bounded(empty,1,0));
    std::cout << "size=" << values.size() << " last=" << values.back() << " limit=preserved\n";
}
