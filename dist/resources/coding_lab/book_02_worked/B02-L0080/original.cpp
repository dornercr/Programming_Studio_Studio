#include <iostream>
#include <string>
#include <tuple>
auto user(){return std::tuple<int,std::string,bool>{7,"Ada",true};}
int main(){ auto [id,name,active]=user(); std::cout<<id<<' '<<name<<' '<<std::boolalpha<<active<<'\n'; }
