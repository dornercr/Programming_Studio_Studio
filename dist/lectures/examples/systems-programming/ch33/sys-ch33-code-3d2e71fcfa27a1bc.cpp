#include <cstddef>
#include <iostream>
int main() {
    const std::size_t file_size = 56, header = 8, record = 12;
    if (file_size < header) return 1;
    const auto payload = file_size - header;
    if (payload % record != 0) return 1;
    const auto count = payload / record;
    const std::size_t index = 2;
    if (index >= count) return 1;
    std::cout << "records=" << count << '\n';
    std::cout << "record 2 offset=" << header + index * record << '\n';
}
