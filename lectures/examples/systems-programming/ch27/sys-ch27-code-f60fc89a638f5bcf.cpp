#include <algorithm>
#include <array>
#include <iostream>
int main() {
    constexpr int requests = 3, service = 5;
    std::array<int,2> available{0,0};
    for (int request=0; request<requests; ++request) {
        auto lane = std::min_element(available.begin(),available.end());
        *lane += service;
    }
    std::cout << "service=" << service << " serial=" << requests*service
              << " two-lanes=" << *std::max_element(available.begin(),available.end()) << '\n';
}
