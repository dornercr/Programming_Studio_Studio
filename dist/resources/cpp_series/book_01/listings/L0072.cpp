#include <iostream>
#include <string>
int main(){
    std::string name = "Ada";
    name += " Lovelace";
    name.replace(0, 3, "Augusta Ada");
    std::cout << name << '\n';
}
