#include <array>
#include <iostream>

int main() {
    const std::array bounds{10,50,100};std::array<unsigned,4> buckets{};
    for(int ms:std::array{3,8,20,80,150}){std::size_t b=0;while(b<bounds.size()&&ms>bounds[b])++b;++buckets[b];}
    std::cout<<"<=10="<<buckets[0]<<" (10,50]="<<buckets[1]<<" (50,100]="<<buckets[2]<<" >100="<<buckets[3]<<'\n';
}
