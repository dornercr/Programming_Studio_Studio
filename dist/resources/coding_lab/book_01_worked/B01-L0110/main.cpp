#include <fstream>
#include <iostream>
void write_report(){
    std::ofstream out("report.txt");
    out << "complete\n";
}
int main(){ write_report(); std::ifstream in("report.txt"); std::string s; std::getline(in,s); std::cout << s << '\n'; }
