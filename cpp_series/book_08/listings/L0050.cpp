#include <iostream>

int main() {
    unsigned desired=3,created=3,ready=1;
    bool api_accepted=desired==created,service_capacity=ready>=2;
    std::cout<<std::boolalpha<<"objects-created="<<api_accepted<<" minimum-ready="<<service_capacity<<" ready="<<ready<<'/'<<desired<<'\n';
}
