#include <iostream>
#include <memory>
#include <string>
int main() {
    std::weak_ptr<std::string> observer;
    {
        auto owner = std::make_shared<std::string>("ready");
        observer = owner;
        std::cout << "alive=" << !observer.expired() << '\n';
    } // The only strong owner ends; the string is destroyed.
    std::cout << "expired=" << observer.expired() << '\n';
    if (auto owner = observer.lock()) std::cout << *owner << '\n';
    else std::cout << "unavailable\n";
}
