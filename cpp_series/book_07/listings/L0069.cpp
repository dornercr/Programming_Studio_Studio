#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    const std::uint16_t version=1;const std::uint32_t count=0x01020304;
    std::array<std::uint8_t,6> wire{std::uint8_t(version>>8),std::uint8_t(version),
        std::uint8_t(count>>24),std::uint8_t(count>>16),std::uint8_t(count>>8),std::uint8_t(count)};
    std::uint32_t decoded=0;for(std::size_t i=2;i<6;++i)decoded=(decoded<<8)|wire[i];
    if(decoded!=count)return 1;
    std::cout<<std::hex<<std::setfill('0');for(auto b:wire)std::cout<<std::setw(2)<<unsigned(b);std::cout<<'\n';
}
