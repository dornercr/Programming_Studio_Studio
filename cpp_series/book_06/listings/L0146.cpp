#include <array>
#include <complex>
#include <numbers>
#include <cmath>
#include <iostream>

int main() {
    constexpr std::size_t n=8;std::array<std::complex<double>,n>x{},spectrum{};
    for(std::size_t t=0;t<n;++t)x[t]=std::polar(1.0,2*std::numbers::pi*2*double(t)/n);
    for(std::size_t k=0;k<n;++k)for(std::size_t t=0;t<n;++t)
        spectrum[k]+=x[t]*std::polar(1.0,-2*std::numbers::pi*double(k*t)/n);
    for(std::size_t k=0;k<n;++k)
        if(std::abs(spectrum[k]-(k==2?std::complex<double>{8,0}:std::complex<double>{0,0}))>1e-12)return 1;
    std::cout << "bin2 magnitude=8 phase approximately0; other bins approximately0\n";
}
