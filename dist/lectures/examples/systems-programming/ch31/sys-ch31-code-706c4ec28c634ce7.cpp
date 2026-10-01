#include <algorithm>
#include <iostream>
#include <vector>
int misses(std::size_t capacity) {
    const std::vector<int> trace{1,2,3,4,1,2,5,1,2,3,4,5};
    std::vector<int> resident;
    int count = 0;
    for (int block : trace) {
        if (std::find(resident.begin(), resident.end(), block)
            != resident.end()) continue; // FIFO hits do not reorder.
        ++count;
        if (resident.size() == capacity) resident.erase(resident.begin());
        resident.push_back(block);
    }
    return count;
}
int main() {
    std::cout << "FIFO slots=3 misses=" << misses(3) << '\n';
    std::cout << "FIFO slots=4 misses=" << misses(4) << '\n';
}
