#include <iostream>
#include <string>
int main() {
    const std::string file = "ABCDEFGH";
    std::size_t kernel_position = 0;
    const auto buffer = file.substr(kernel_position, 4);
    kernel_position += buffer.size();
    std::size_t consumed = 0;
    const char first = buffer.at(consumed++);
    const char buffered_next = buffer.at(consumed);
    const char raw_next = file.at(kernel_position);
    std::cout << "first=" << first << '\n';
    std::cout << "buffered-next=" << buffered_next << '\n';
    std::cout << "raw-next=" << raw_next << '\n';
}
