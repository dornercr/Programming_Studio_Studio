#include <array>
#include <cstring>
#include <iostream>
#include <string_view>
static_assert('A' == 65 && 'B' == 66, "ASCII fixture required");
int main() {
    const std::array<char, 4> storage{'A', '\0', 'B', '\0'};
    const std::string_view bytes(storage.data(), 3);
    unsigned sum = 0;
    for (unsigned char value : bytes) sum += value;
    std::cout << "c-string-bytes=" << std::strlen(storage.data()) << '\n';
    std::cout << "counted-bytes=" << bytes.size() << " sum=" << sum << '\n';
}
