#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

bool parseField(const std::string& text,int& output){
 int candidate{};auto r=std::from_chars(text.data(),text.data()+text.size(),candidate);
 // BUG: ignores unconsumed trailing bytes.
 if(r.ec!=std::errc{}||candidate<0||candidate>100)return false;
 output=candidate;return true;
}

int main() {
    std::string s;
    std::getline(std::cin,s);
    int n=8;
    if(parseField(s,n))std::cout<<n<<"\n";
    else std::cout<<"invalid "<<n<<"\n";
    }
