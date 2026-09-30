#include <iostream>
int main(){
    std::size_t n=3;
    int* p = new int[n]{10,20,30};
    for(std::size_t i=0;i<n;++i) std::cout << p[i] << ' ';
    delete[] p;
}
