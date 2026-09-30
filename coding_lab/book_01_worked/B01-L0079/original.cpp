#include <iostream>
#include <vector>
int main(){
    std::vector<int> v;
    for(int i=0;i<8;++i){
        v.push_back(i);
        std::cout << "size=" << v.size() << " capacity=" << v.capacity() << '\n';
    }
}
