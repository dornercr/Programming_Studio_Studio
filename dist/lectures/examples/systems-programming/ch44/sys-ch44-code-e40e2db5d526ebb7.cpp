#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
int main() {
    std::exception_ptr failure;
    std::jthread worker([&failure] {
        try { throw std::runtime_error("bad sample"); }
        catch (...) { failure = std::current_exception(); }
    });
    worker.join();
    try {
        if (failure) std::rethrow_exception(failure);
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << '\n';
    }
}
