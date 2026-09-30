#include <vector>
#include <string>
#include <iostream>
#include <utility>

int main() {
    struct Membership{unsigned generation=0;std::vector<std::string> instances;
        bool replace(unsigned g,std::vector<std::string> next){if(g<=generation)return false;generation=g;instances=std::move(next);return true;}};
    Membership m;m.replace(7,{"worker-a@10.0.0.1:7000"});
    bool stale=m.replace(6,{"retired@10.0.0.9:7000"});
    std::cout<<"generation="<<m.generation<<" stale-accepted="<<std::boolalpha<<stale<<" instance="<<m.instances.front()<<'\n';
}
