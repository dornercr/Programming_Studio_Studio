#include <map>
#include <optional>
#include <string>
#include <cassert>

struct Repo{virtual ~Repo()=default;virtual void put(int,std::string)=0;virtual std::optional<std::string> get(int)=0;};
struct MemoryRepo:Repo{std::map<int,std::string> m;void put(int k,std::string v)override{m[k]=std::move(v);}std::optional<std::string> get(int k)override{auto i=m.find(k);if(i==m.end())return {};return i->second;}};
int main(){MemoryRepo r;r.put(7,"seven");assert(r.get(7)=="seven");assert(!r.get(8));}
