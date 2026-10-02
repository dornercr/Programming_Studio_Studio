#include <iostream>
#include <ranges>
#include <vector>
int main(){ std::vector<int> v{1,2,3,4,5}; auto view=v|std::views::filter([](int x){return x%2==1;})|std::views::transform([](int x){return x*x;}); for(int x:view)std::cout<<x<<' '; }
