#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
int main() {
    std::istringstream input("6 4");
    std::vector<int> values;
    int value = 0;
    // Suitable for this fixed input, not a complete token grammar.
    while (input >> value) values.push_back(value);
    if (values.empty() || values.size() > 1000)
        throw std::invalid_argument("count");
    for (int n : values) {
        if (n < -100 || n > 100)
            throw std::invalid_argument("range");
    }
    int total = 0;
    for (int n : values) total += n;
    std::cout << "count=" << values.size()
              << " total=" << total << '\n';
}
