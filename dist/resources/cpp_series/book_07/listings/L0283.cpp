#include <algorithm>
#include <array>
#include <iostream>

int main() {
    constexpr double cpu_cores=4,cpu_seconds_per_request=0.002;
    double cpu_limit=cpu_cores/cpu_seconds_per_request;
    double database_limit=1200,downstream_limit=1600;
    double system_limit=std::min({cpu_limit,database_limit,downstream_limit});
    std::cout<<"cpu="<<cpu_limit<<" database="<<database_limit<<" downstream="<<downstream_limit<<" limiting="<<system_limit<<" req/s\n";
}
