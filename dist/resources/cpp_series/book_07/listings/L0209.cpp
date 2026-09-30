#include <algorithm>
#include <iostream>

int main() {
    double tokens=2,last=0;unsigned in_flight=0;
    auto admit=[&](double now){tokens=std::min(2.0,tokens+(now-last)*1.0);last=now;
        if(tokens<1||in_flight==1)return false;tokens-=1;++in_flight;return true;};
    bool a=admit(0),b=admit(0);--in_flight;bool c=admit(0);--in_flight;bool d=admit(0),e=admit(1);
    std::cout<<std::boolalpha<<a<<' '<<b<<' '<<c<<' '<<d<<' '<<e<<'\n';
}
