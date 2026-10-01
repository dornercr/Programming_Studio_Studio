#include <iostream>
#include <string>

int main() {
    const std::string body = "ready now";
    const std::string prefix = "WARN ";
    const auto prefix_last = prefix + body.substr(0, 5);
    const auto limit_last = (prefix + body).substr(0, 5);
    std::cout << "prefix last='" << prefix_last
              << "' size=" << prefix_last.size() << '\n';
    std::cout << "limit last='" << limit_last
              << "' size=" << limit_last.size() << '\n';
}
