#include <array>
#include <complex>
#include <numbers>
#include <cmath>
#include <iostream>

int main() {
    std::array<std::complex<double>,4> iq{};double power=0;
    for(std::size_t n=0;n<iq.size();++n){
        double phase=2*std::numbers::pi*double(n)/4;
        iq[n]=std::polar(2.0,phase);power+=std::norm(iq[n]);
        if(std::abs(std::abs(iq[n])-2)>1e-12)return 1;
    }
    std::cout << "samples=" << iq.size() << " amplitude=2 mean-power=" << power/iq.size() << '\n';
}
