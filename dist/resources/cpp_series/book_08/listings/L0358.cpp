#include <map>
#include <optional>
#include <string>
#include <iostream>

int main() {
    struct Job{int input;std::optional<int> result;};
    std::map<std::string,Job> jobs;
    auto submit=[&](std::string id,int input){
        if(input<0||input>100)return std::string("invalid");
        if(auto i=jobs.find(id);i!=jobs.end())return std::string(i->second.input==input?"replay":"conflict");
        if(jobs.size()==2)return std::string("busy");
        jobs.emplace(std::move(id),Job{input,{}});return std::string("accepted");};
    std::cout<<submit("j1",7)<<' '<<submit("j1",7)<<' '<<submit("j1",8)<<'\n';
    for(auto&[id,j]:jobs)if(!j.result)j.result=j.input*j.input;
    std::cout<<"result="<<*jobs.at("j1").result<<'\n';
}
