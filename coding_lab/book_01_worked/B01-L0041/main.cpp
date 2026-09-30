#include <iostream>
int main(){
int count=4;
const int before = count;
++count;
const int after = count;
std::cout<<before<<" "<<after<<"\n";
}
