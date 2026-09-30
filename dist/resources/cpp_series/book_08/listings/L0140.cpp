#include <memory>
#include <vector>
#include <string>
#include <iostream>

int main() {
    auto current=std::make_shared<const std::vector<std::string>>(std::vector<std::string>{"worker-a:8080"});
    auto operation=current;current=std::make_shared<const std::vector<std::string>>(std::vector<std::string>{"worker-b:8080"});
    std::cout<<"in-flight="<<operation->front()<<" new-request="<<current->front()<<'\n';
}
