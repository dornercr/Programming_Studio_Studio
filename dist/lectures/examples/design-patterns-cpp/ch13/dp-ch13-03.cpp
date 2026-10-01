#include <iostream>
#include <optional>
#include <string>
#include <vector>
using Reply = std::optional<std::string>;
std::string first_answer(const std::vector<Reply>& replies) {
    for (const auto& reply : replies) {
        if (reply) return *reply;
    }
    return "unhandled";
}
std::string last_answer(const std::vector<Reply>& replies) {
    std::string result = "unhandled";
    for (const auto& reply : replies) {
        if (reply) result = *reply; // Intentional policy change.
    }
    return result;
}
int main() {
    // Precomputed replies isolate the stopping rule from handler code.
    const std::vector<Reply> replies{"label desk", "general desk"};
    std::cout << "first=" << first_answer(replies) << '\n';
    std::cout << "last=" << last_answer(replies) << '\n';
}
