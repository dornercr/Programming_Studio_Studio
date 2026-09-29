#include <array>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> jobs{4};constexpr std::size_t capacity=2;
    auto submit=[&](int x){if(x<0||x>100||jobs.size()==capacity)return false;jobs.push_back(x);return true;};
    bool a=submit(7),b=submit(101),c=submit(8);
    if(jobs!=std::vector<int>{4,7})return 1;
    std::cout<<std::boolalpha<<"accepted="<<a<<" invalid="<<b<<" full="<<c<<" count="<<jobs.size()<<'\n';
}
