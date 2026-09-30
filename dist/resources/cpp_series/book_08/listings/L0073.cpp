#include <charconv>
#include <string_view>
#include <iostream>

int main() {
    unsigned workers=4;
    auto reload=[&](std::string_view s){unsigned n=0;auto[p,e]=std::from_chars(s.data(),s.data()+s.size(),n);
        if(e!=std::errc{}||p!=s.data()+s.size()||n<1||n>32)return false;workers=n;return true;};
    bool bad=reload("1000"),good=reload("8");
    std::cout<<"invalid="<<std::boolalpha<<bad<<" valid="<<good<<" workers="<<workers<<'\n';
}
