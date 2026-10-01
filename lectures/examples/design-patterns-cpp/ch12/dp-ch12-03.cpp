#include <iostream>
#include <optional>
#include <stdexcept>
int wrong(bool allowed, const std::optional<int>& cached) {
    if (cached) return *cached; // Intentional bypass of revocation.
    if (!allowed) throw std::runtime_error("denied");
    return 8;
}
int checked(bool allowed, const std::optional<int>& cached) {
    if (!allowed) throw std::runtime_error("denied");
    if (cached) return *cached;
    return 8;
}
int main() {
    const std::optional<int> warm_cache{8};
    std::cout << "wrong=" << wrong(false, warm_cache) << '\n';
    try {
        const int answer = checked(false, warm_cache);
        std::cout << "checked=" << answer << '\n';
    } catch (const std::runtime_error& error) {
        std::cout << "checked=" << error.what() << '\n';
    }
}
