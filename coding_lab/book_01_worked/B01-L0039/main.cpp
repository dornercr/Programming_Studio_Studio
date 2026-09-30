#include <iostream>
int main(){
int count=5,maximum=10;
const bool valid = count >= 0 && count <= maximum;
std::cout<<std::boolalpha<<valid<<"\n";
}
