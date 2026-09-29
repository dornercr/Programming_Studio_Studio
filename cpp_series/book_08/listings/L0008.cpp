#include <map>
#include <optional>
#include <string>
#include <iostream>

int main() {
    struct Job{int input;std::optional<int> result;};std::map<std::string,Job> durable{{"a",{7,{}}}};
    bool volatile_in_flight=true;volatile_in_flight=false; // process state lost
    for(auto&[id,j]:durable)if(!j.result){volatile_in_flight=true;j.result=j.input*j.input;volatile_in_flight=false;}
    std::cout<<"recovered="<<*durable.at("a").result<<" in-flight="<<std::boolalpha<<volatile_in_flight<<'\n';
}
