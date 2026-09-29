#include <memory>
#include <string>
#include <string_view>
#include <iostream>
#include <utility>

int main() {
    struct Payload{std::unique_ptr<std::string> data;
        explicit Payload(std::string s):data(std::make_unique<std::string>(std::move(s))){}
        std::string_view view()const{return data?std::string_view(*data):std::string_view{};}};
    Payload a("job7");Payload b(std::move(a));
    std::cout<<"source-empty="<<std::boolalpha<<a.view().empty()<<" destination="<<b.view()<<'\n';
}
