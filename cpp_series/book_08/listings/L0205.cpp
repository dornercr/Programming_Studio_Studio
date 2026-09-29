#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <iostream>

int main() {
    auto quote=[](std::string_view s){std::ostringstream out;out<<'"';for(unsigned char c:s){
        if(c=='"'||c=='\\')out<<'\\'<<char(c);else if(c<0x20)out<<"\\u"<<std::hex<<std::setw(4)<<std::setfill('0')<<unsigned(c);else out<<char(c);}out<<'"';return out.str();};
    std::string request="job\n7";std::string token="not-for-logs";(void)token;
    std::cout<<"{\"event\":\"rejected\",\"request\":"<<quote(request)<<",\"reason\":\"syntax\"}\n";
}
