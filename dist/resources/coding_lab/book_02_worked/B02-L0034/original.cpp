#include <iostream>
#include <string>
struct Device{ std::string id; explicit Device(std::string x):id(std::move(x)){} };
struct Sensor:Device{ double value; Sensor(std::string id,double v):Device(std::move(id)),value(v){} };
int main(){ Sensor s{"T1",21.5}; std::cout<<s.id<<' '<<s.value<<'\n'; }
