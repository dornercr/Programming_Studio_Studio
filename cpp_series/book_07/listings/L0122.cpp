#include <string>
#include <iostream>
#include <cstdint>

int main() {
    struct Snapshot{std::uint64_t fence;std::string value;};
    Snapshot durable{44,"committed"};Snapshot restored=durable;
    auto write=[&](std::uint64_t f,std::string v){if(f<restored.fence)return false;restored={f,std::move(v)};return true;};
    bool stale=write(43,"obsolete"),current=write(45,"replacement");
    std::cout<<std::boolalpha<<"stale="<<stale<<" current="<<current<<" fence="<<restored.fence<<'\n';
}
