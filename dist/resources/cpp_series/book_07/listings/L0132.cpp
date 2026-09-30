#include <string>
#include <iostream>

int main() {
    enum class Phase{copying,caught_up,frozen,cutover};Phase phase=Phase::copying;
    int source=5,destination=source;std::string owner="old";
    source+=2;destination=source;phase=Phase::caught_up;
    phase=Phase::frozen;bool late_write=phase!=Phase::frozen;
    if(destination==source){owner="new";phase=Phase::cutover;}
    std::cout<<"source="<<source<<" destination="<<destination<<" late-write="<<std::boolalpha<<late_write<<" owner="<<owner<<'\n';
}
