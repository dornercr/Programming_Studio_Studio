#include <array>
#include <iostream>
#include <string_view>

int main() {
    const std::array<std::string_view,3> outcomes{"ok","invalid","busy"};std::array<unsigned,3> counts{2,1,0};
    for(std::size_t i=0;i<outcomes.size();++i)
        std::cout<<"harbor_requests_total{outcome=\""<<outcomes[i]<<"\"} "<<counts[i]<<'\n';
}
