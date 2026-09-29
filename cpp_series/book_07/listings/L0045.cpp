#include "harbor/check.hpp"
#include <memory>
#include <iostream>
struct Settings { unsigned workers; unsigned queue; };
std::shared_ptr<const Settings> validate(Settings candidate) {
    if (!candidate.workers || candidate.workers>64 || !candidate.queue || candidate.queue>4096)
        throw std::invalid_argument("configuration bounds");
    return std::make_shared<const Settings>(candidate);
}
int main() {
    auto current=validate({2,16});
    auto request_snapshot=current;
    harbor::rejects([&]{ current=validate({0,16}); },"invalid reload");
    harbor::check(current->workers==2,"failed reload changed config");
    current=validate({4,32});
    harbor::check(request_snapshot->workers==2 && current->workers==4,"snapshot lifetime");
    std::cout<<"Validated snapshot replaced; existing request retained its policy.\n";
}
