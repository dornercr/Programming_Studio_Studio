#include <array>
#include <iostream>

int main() {
    unsigned requests=0,in_flight=0;std::array<unsigned,3> buckets{};double sum=0;
    const std::array<double,3> bounds{0.01,0.1,1.0};
    for(double seconds:std::array{0.005,0.050,0.200}){++in_flight;++requests;sum+=seconds;
        for(std::size_t i=0;i<bounds.size();++i)if(seconds<=bounds[i])++buckets[i];--in_flight;}
    std::cout<<"requests="<<requests<<" in-flight="<<in_flight<<" buckets="<<buckets[0]<<','<<buckets[1]<<','<<buckets[2]<<" sum_s="<<sum<<'\n';
}
