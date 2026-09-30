#include <iostream>
#include <string>
int main(){

const int id{42};
const std::string label = std::string{"sensor="}
                        + std::to_string(id);
std::cout<<label<<"\n";
}
