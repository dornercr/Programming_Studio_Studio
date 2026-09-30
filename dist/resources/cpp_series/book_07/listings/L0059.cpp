#include <array>
#include <string_view>
#include <iostream>
#include <algorithm>

int main() {
    struct Connection{std::array<char,8> input{};std::size_t used=0;int deadline_ms;
        bool append(std::string_view bytes,int now){if(now>=deadline_ms||bytes.size()>input.size()-used)return false;
            std::copy(bytes.begin(),bytes.end(),input.begin()+used);used+=bytes.size();return true;}};
    Connection a{{},0,100},b{{},0,100};
    bool first=a.append("PING",10),large=a.append("TOO-LONG",20),other=b.append("OK",30);
    std::cout<<std::boolalpha<<first<<' '<<large<<' '<<other<<" a-bytes="<<a.used<<" b-bytes="<<b.used<<'\n';
}
