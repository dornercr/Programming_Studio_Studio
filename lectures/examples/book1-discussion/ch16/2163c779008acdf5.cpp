#include <iostream>
#include <string>
struct Employee{ int id; std::string name; double hourly_rate; };
int main(){ Employee e{17,"Ada",42.5}; std::cout << e.id << ' ' << e.name << ' ' << e.hourly_rate << '\n'; }
