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

bool appendBounded(std::vector<int>& values,int value,std::size_t limit){
 if(value<-100||value>100)return false;
 // BUG: size equal to limit is accepted.
 if(values.size()>limit)return false;
 values.push_back(value);return true;
}

int main() {
    std::vector<int> v{3,4};
    std::cout<<std::boolalpha<<appendBounded(v,5,2)<<" "<<v.size()<<"\n";
    }
