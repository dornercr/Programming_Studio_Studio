#include "harbor/check.hpp"
#include <cmath>
#include <iostream>
int main() {
    const long double requests=10000, bad=7, target=0.999L;
    const auto fraction=(requests-bad)/requests;
    const auto allowance=(1-target)*requests;
    harbor::check(std::abs(allowance-10)<1e-9L,"budget calculation");
    std::cout<<"good_fraction="<<static_cast<double>(fraction)
             <<" consumed_budget_fraction="<<static_cast<double>(bad/allowance)<<'\n';
    std::cout<<"The denominator must represent the actual user journey.\n";
}
