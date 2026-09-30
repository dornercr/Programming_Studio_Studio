#include <string_view>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <optional>
#include <algorithm>

int main() {
    struct Job{std::string_view key;std::int64_t input;};
    auto parse=[](std::string_view key,std::string_view text)->std::optional<Job>{
        if(key.empty()||key.size()>64||!std::all_of(key.begin(),key.end(),[](unsigned char c){return(c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-';}))return{};
        std::int64_t x=0;auto[p,e]=std::from_chars(text.data(),text.data()+text.size(),x);
        if(e!=std::errc{}||p!=text.data()+text.size()||x<0||x>1000000)return{};return Job{key,x};};
    auto good=parse("job_7","12"),bad=parse("bad key","12");
    if(!good||bad)return 1;std::int64_t worker_result=144;
    std::cout<<"gateway-valid="<<good->key<<" worker-result-verified="<<std::boolalpha<<(worker_result==good->input*good->input)<<'\n';
}
