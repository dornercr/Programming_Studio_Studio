#include "routes.hpp"
#include <exception>
#include <iostream>
int main() {
    try {
        const auto request = harbor::parse(std::cin);
        harbor::write(std::cout, harbor::shortest_route(request));
        std::cout.flush();
        if (!std::cout) throw std::runtime_error("output failure");
        return 0;
    } catch (const harbor::InputError& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 3;
    }
}
