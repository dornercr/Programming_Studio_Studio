#include <iostream>
#include <string_view>
enum class Region { Invalid, Anonymous, ReadOnly, CopyOnWrite };
std::string_view decide(Region region, bool present, bool write) {
    if (region == Region::Invalid) return "deny";
    if (write && region == Region::ReadOnly) return "deny";
    if (write && region == Region::CopyOnWrite) return "private-write";
    if (!present && region == Region::Anonymous) return "demand-zero";
    return present ? "resume" : "needs-backing-policy";
}
int main() {
    std::cout << decide(Region::Anonymous,false,false) << '\n';
    std::cout << decide(Region::ReadOnly,true,true) << '\n';
    std::cout << decide(Region::CopyOnWrite,true,true) << '\n';
    std::cout << decide(Region::ReadOnly,true,false) << '\n';
}
