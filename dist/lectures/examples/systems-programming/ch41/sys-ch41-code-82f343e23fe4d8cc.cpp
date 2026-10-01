#include <iostream>
int main() {
    // Serialized model only: no real signals or sleeping occur here.
    bool request = false;
    const bool need_to_wait = !request;
    request = true; // Delivery between check and sleep.
    const bool asleep = need_to_wait;
    bool protected_request = false;
    const bool pending = true; // Delivery is held pending during checking.
    bool blocked = !protected_request;
    if (pending) { protected_request = true; blocked = false; }
    std::cout << std::boolalpha;
    std::cout << "broken sleeps=" << asleep << " request=" << request << '\n';
    std::cout << "coordinated blocked=" << blocked
              << " request=" << protected_request << '\n';
}
