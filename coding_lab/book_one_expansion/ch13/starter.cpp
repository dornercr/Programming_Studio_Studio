#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

bool shiftAll(std::vector<int>& values,int delta){
 // BUG: an early value changes before a later rejection.
 for(int& value:values){if(value+delta<-100||value+delta>100)return false;value+=delta;}
 return true;
}

int main() {
    std::vector<int> v{1,99};
    bool ok=shiftAll(v,2);
    std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\n";
    }
