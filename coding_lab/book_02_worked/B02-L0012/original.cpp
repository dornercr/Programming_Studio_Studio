#include <fstream>
#include <iostream>
void write(){ std::ofstream out("resource.txt"); out << "data\n"; }
int main(){ write(); std::ifstream in("resource.txt"); std::string s; in>>s; std::cout<<s<<'\n'; }
