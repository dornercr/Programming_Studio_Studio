#include "check.hpp"
#include <chrono>
#include <exception>
#include <future>
#include <stdexcept>
#include <thread>

int checked_square(int value) {
    if (value < -1000 || value > 1000) throw std::out_of_range("square domain");
    return value * value;
}
int main() {
    std::promise<int> producer;
    auto result = producer.get_future();
    std::jthread worker([promise = std::move(producer)]() mutable {
        try { promise.set_value(checked_square(12)); }
        catch (...) { promise.set_exception(std::current_exception()); }
    });
    CHECK(result.get() == 144);
    worker.join();
    auto failed = std::async(std::launch::async, checked_square, 2000);
    bool rejected = false;
    try { (void)failed.get(); } catch (const std::out_of_range&) { rejected = true; }
    CHECK(rejected);
    auto deferred = std::async(std::launch::deferred, checked_square, 7);
    CHECK(deferred.wait_for(std::chrono::seconds{0}) == std::future_status::deferred);
    CHECK(deferred.get() == 49);
    std::future<int> abandoned;
    { std::promise<int> incomplete; abandoned = incomplete.get_future(); }
    bool broken = false;
    try { (void)abandoned.get(); }
    catch (const std::future_error& error) {
        broken = error.code() == std::make_error_code(std::future_errc::broken_promise);
    }
    CHECK(broken);
    std::cout << "PASS\n";
}
