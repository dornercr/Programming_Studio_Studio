#include <fstream>
#include <iostream>
#include <string>
int main(){ {std::ofstream out("note.txt"); if(!out)return 1; out<<"hello\n";} std::ifstream in("note.txt"); std::string line; if(std::getline(in,line))std::cout<<line<<'\n'; }
