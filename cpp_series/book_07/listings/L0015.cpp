#include <functional>
#include <string>
#include <iostream>
#include <stdexcept>

int main() {
    using Service=std::function<int(int)>;
    Service local=[](int x){return x*x;};
    Service serialized=[&](int x){std::string wire=std::to_string(x);return local(std::stoi(wire));};
    for(const auto* service:{&local,&serialized})if((*service)(9)!=81)return 1;
    std::cout<<"local and serialized adapters preserve81\n";
}
