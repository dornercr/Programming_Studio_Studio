#include <fstream>
#include <string>
#include <iostream>

int main() {
    {std::ofstream out("config.txt");out<<"workers=4\n";if(!out)return 1;}
    std::ifstream in("config.txt");std::string line;
    if(!std::getline(in,line)||line!="workers=4")return 2;
    std::cout<<"native configuration read: "<<line<<'\n';
}
