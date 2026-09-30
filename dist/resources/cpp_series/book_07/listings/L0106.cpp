#include <optional>
#include <iostream>

int main() {
    struct Replica{unsigned applied;int value;
        std::optional<int> read(unsigned required)const{if(applied<required)return{};return value;}};
    Replica stale{7,10},fresh{8,20};unsigned client_version=8;
    auto a=stale.read(client_version),b=fresh.read(client_version);
    std::cout<<"stale-rejected="<<std::boolalpha<<!a<<" current="<<*b<<'\n';
}
