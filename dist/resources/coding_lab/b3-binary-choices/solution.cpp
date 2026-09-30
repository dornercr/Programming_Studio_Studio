#include <iostream>
#include <vector>
#include <string>
#include <cstddef>
#include <stdexcept>

void visit(std::size_t n,std::string& path,std::vector<std::string>& out){
    if(path.size()==n){out.push_back(path);return;}
    for(char choice:{'0','1'}){
        path.push_back(choice);visit(n,path,out);path.pop_back();
    }
}
std::vector<std::string> binaries(std::size_t n){
    if(n>10) throw std::invalid_argument("too long");
    std::string path;std::vector<std::string> out;visit(n,path,out);return out;
}
