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

int priceTotal(int unit,int count){
 if(unit<0||unit>1000||count<0||count>100)throw std::invalid_argument("range");
 // BUG: adds instead of pricing the quantity.
 return unit+count;
}

int main() {
    int u,n;
    if(!(std::cin>>u>>n))return 2;
    try{std::cout<<priceTotal(u,n)<<"\n";
    }catch(const std::exception& e){std::cout<<e.what()<<"\n";
    }}
