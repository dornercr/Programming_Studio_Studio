#include <optional>
#include <string>
#include <iostream>

int main() {
    std::optional<std::string> decided;
    auto decide=[&](std::string value,unsigned votes){
        if(decided)return *decided==value;
        if(votes<2)return false;
        decided=std::move(value);return true;
    };
    bool waiting=decide("A",1),committed=decide("A",2),conflict=decide("B",3);
    std::cout<<std::boolalpha<<waiting<<' '<<committed<<' '<<conflict<<" decision="<<*decided<<'\n';
}
