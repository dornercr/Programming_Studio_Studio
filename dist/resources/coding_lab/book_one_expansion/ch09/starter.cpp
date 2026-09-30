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

int nextTicket(int& counter){
 if(counter<0||counter>999)throw std::invalid_argument("range");
 // BUG: reads the state but never advances it.
 return counter;
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    try{int issued=nextTicket(n);
    std::cout<<issued<<" "<<n<<"\n";
    }catch(const std::invalid_argument&){std::cout<<"range\n";
    }}
