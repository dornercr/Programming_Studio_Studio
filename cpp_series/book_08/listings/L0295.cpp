#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

int main() {
    struct ObjectStore{virtual ~ObjectStore()=default;virtual void put(std::string key,std::string value)=0;};
    struct MemoryStore final:ObjectStore{std::map<std::string,std::string> objects;void put(std::string k,std::string v)override{objects[std::move(k)]=std::move(v);}};
    auto archive=[](ObjectStore&store,std::string_view id){store.put(std::string(id),"completed");};
    MemoryStore fake;archive(fake,"job-7");
    std::cout<<"stored="<<fake.objects.at("job-7")<<"; domain API contains no credential parameter\n";
}
