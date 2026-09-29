#include <array>
#include <iostream>

int main() {
    struct Build{int minimum_schema;int maximum_schema;};
    auto reads=[](Build b,int schema){return b.minimum_schema<=schema&&schema<=b.maximum_schema;};
    Build old{1,2},next{2,3};
    for(int schema:std::array{1,2,3})
        std::cout<<"schema="<<schema<<" coexist="<<std::boolalpha
                 <<(reads(old,schema)&&reads(next,schema))<<'\n';
}
