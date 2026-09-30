#include <map>
#include <string>
#include <iostream>

int main() {
    const std::map<std::string,int> durable{{"a",16},{"b",25}};
    std::map<std::string,int> cache;unsigned requests=9;
    cache=durable;requests=0; // modeled process restart
    cache.erase("a"); // eviction is allowed
    auto value=[&](const std::string& id){++requests;return durable.at(id);};
    std::cout<<"a="<<value("a")<<" cached="<<cache.size()<<" requests-since-restart="<<requests<<'\n';
}
