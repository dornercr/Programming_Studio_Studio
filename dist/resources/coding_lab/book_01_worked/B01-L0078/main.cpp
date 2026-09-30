#include <algorithm>
#include <array>
#include <iostream>
int main(){
    std::array<int,4> a{4,1,3,2};
    std::sort(a.begin(), a.end());
    for(int v:a) std::cout << v << ' ';
}
