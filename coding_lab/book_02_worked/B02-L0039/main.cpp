#include <iostream>
#include <string>
#include <unordered_map>
struct Store{ virtual ~Store()=default; virtual void put(std::string,int)=0; virtual int get(const std::string&) const=0; };
struct MemoryStore:Store{ std::unordered_map<std::string,int> m; void put(std::string k,int v) override{m[std::move(k)]=v;} int get(const std::string& k) const override{return m.at(k);} };
int main(){ MemoryStore s; s.put("x",7); std::cout<<s.get("x")<<'\n'; }
