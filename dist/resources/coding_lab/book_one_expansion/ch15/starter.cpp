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

std::unique_ptr<int[]> makeSequence(std::size_t count){
 if(count>8)throw std::invalid_argument("range");if(count==0)return {};
 auto out=std::make_unique<int[]>(count);
 // BUG: values remain zero-initialized.
 return out;
}

int main() {
    auto p=makeSequence(3);
    std::cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<"\n";
    }
