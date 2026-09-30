#include <iostream>
int main(){
    int a[]{10,20,30};
    int* p=a;
    for(std::size_t i=0;i<3;++i) std::cout << *(p+i) << ' ';
}
