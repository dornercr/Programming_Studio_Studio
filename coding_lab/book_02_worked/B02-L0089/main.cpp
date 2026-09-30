#include <iostream>
#include <optional>
#include <ranges>
#include <vector>
std::optional<int> first_even(const std::vector<int>& v){ auto evens=v|std::views::filter([](int x){return x%2==0;}); auto it=evens.begin(); if(it==evens.end())return std::nullopt; return *it; }
int main(){ std::vector<int> v{1,3,8,10}; if(auto x=first_even(v))std::cout<<*x<<'\n'; }
