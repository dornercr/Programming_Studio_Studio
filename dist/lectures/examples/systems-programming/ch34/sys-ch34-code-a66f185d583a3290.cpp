#include <iostream>
#include <string>
int main() {
    const std::string payload = "ABCDEF";
    const std::size_t completed = 3;
    const std::string delivered = payload.substr(0, completed);
    // Model a later failure after the prefix has already been accepted.
    const auto wrong_restart = delivered + payload;
    const auto resumed = delivered + payload.substr(completed);
    std::cout << "before retry=" << delivered << '\n';
    std::cout << "wrong restart=" << wrong_restart << '\n';
    std::cout << "resume=" << resumed << '\n';
}
