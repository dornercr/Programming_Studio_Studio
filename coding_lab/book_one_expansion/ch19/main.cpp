#include "billing.hpp"
#include <iostream>
#include <stdexcept>
int main() {
    int u,n;
    if(!(std::cin>>u>>n))return 2;
    try{std::cout<<billing::subtotal(u,n)<<"\n";
    }catch(const std::invalid_argument&){std::cout<<"range\n";
    }}
