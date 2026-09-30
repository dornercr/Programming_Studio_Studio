#include <iostream>
#include <string>
#include <cstddef>

std::string redactLog(std::string text) {
    std::size_t pos=0;
    while((pos=text.find("token=",pos))!=std::string::npos){
        auto start=pos+6;
        auto end=text.find(' ',start);
        if(end==std::string::npos) end=text.size();
        text.replace(start,end-start,"***");
        pos=start+3;
    }
    return text;
}
