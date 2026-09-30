#include <iostream>
#include <array>
int main(){

std::array<int, 3> calibration{1, 2, 3};
for (int& value : calibration) {
    value *= 2;
}
for(int value:calibration)std::cout<<value<<" ";std::cout<<"\n";
}
