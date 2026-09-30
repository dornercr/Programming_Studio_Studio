#include <iostream>
int main(){
int items=23,capacity=5;
const int full = items / capacity;
const int remainder = items % capacity;
const int needed = full + (remainder != 0 ? 1 : 0);
std::cout<<full<<" "<<remainder<<" "<<needed<<"\n";
}
