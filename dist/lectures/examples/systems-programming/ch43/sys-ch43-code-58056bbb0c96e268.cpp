#include <algorithm>
#include <array>
#include <iostream>
#include <string>
int main() {
    // Serialized enumeration of abstract activity steps.
    std::array<char,5> order{'A','A','A','B','B'};
    unsigned count = 0;
    std::string first, last;
    do {
        const std::string trace(order.begin(), order.end());
        if (count == 0) first = trace;
        last = trace;
        ++count;
    } while (std::next_permutation(order.begin(), order.end()));
    std::cout << "schedules=" << count << '\n';
    std::cout << "first=" << first << " last=" << last << '\n';
}
