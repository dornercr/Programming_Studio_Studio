#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

int main() {
    const std::filesystem::path data="lab-volume";std::filesystem::create_directories(data);
    {std::ofstream out(data/"checkpoint.txt");out<<"generation=7\n";if(!out)return 1;}
    std::ifstream in(data/"checkpoint.txt");std::string line;if(!std::getline(in,line))return 2;
    std::cout<<"volume-state="<<line<<'\n';
}
