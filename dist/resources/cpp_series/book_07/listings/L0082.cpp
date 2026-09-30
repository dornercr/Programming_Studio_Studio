#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<std::string,int> receipts;unsigned effects=0;
    auto execute=[&](const std::string&id,int x){auto it=receipts.find(id);if(it!=receipts.end())return it->second;
        ++effects;return receipts.emplace(id,x*x).first->second;};
    (void)execute("job9",6);bool caller_cancelled=true; // reply discarded after commit
    int recovered=execute("job9",6);
    std::cout<<"cancelled="<<std::boolalpha<<caller_cancelled<<" recovered="<<recovered<<" effects="<<effects<<'\n';
}
