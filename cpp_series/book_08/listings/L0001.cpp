#include <array>
#include <string_view>
#include <iostream>

int main() {
    enum class Outcome{accepted,invalid,busy};unsigned queued=0;
    auto submit=[&](int n){if(n<0||n>100)return Outcome::invalid;if(queued==2)return Outcome::busy;++queued;return Outcome::accepted;};
    auto name=[](Outcome o)->std::string_view{return o==Outcome::accepted?"accepted":o==Outcome::invalid?"invalid":"busy";};
    for(int n:std::array{2,-1,3,4})std::cout<<name(submit(n))<<'\n';
}
