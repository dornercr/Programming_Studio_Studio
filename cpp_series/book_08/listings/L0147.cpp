#include <optional>
#include <string>
#include <iostream>

int main() {
    struct Entry{std::string address;int expires;};std::optional<Entry> cached=Entry{"10.0.0.2",100};
    auto resolve=[&](int now,bool refresh_ok)->std::optional<std::string>{if(cached&&now<cached->expires)return cached->address;
        if(!refresh_ok)return{};cached=Entry{"10.0.0.3",now+100};return cached->address;};
    auto a=resolve(99,false),b=resolve(101,false),c=resolve(102,true);
    std::cout<<"before="<<*a<<" expired-unavailable="<<std::boolalpha<<!b<<" refreshed="<<*c<<'\n';
}
