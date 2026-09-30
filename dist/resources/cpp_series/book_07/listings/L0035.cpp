#include "harbor/check.hpp"
#include <memory>
#include <string>
#include <iostream>
class ClientOptions {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    explicit ClientOptions(std::string endpoint);
    ~ClientOptions();
    ClientOptions(ClientOptions&&) noexcept;
    ClientOptions& operator=(ClientOptions&&) noexcept;
    std::string endpoint() const;
};
struct ClientOptions::Impl { std::string endpoint; };
ClientOptions::ClientOptions(std::string e):impl_(std::make_unique<Impl>(Impl{std::move(e)})) {}
ClientOptions::~ClientOptions()=default;
ClientOptions::ClientOptions(ClientOptions&&) noexcept=default;
ClientOptions& ClientOptions::operator=(ClientOptions&&) noexcept=default;
std::string ClientOptions::endpoint() const {
    if (!impl_) throw std::logic_error("moved-from options");
    return impl_->endpoint;
}
int main() {
    ClientOptions a("loopback"); ClientOptions b(std::move(a));
    harbor::check(b.endpoint()=="loopback","pimpl move");
    harbor::rejects([&]{ (void)a.endpoint(); },"moved-from contract");
    std::cout<<"Representation is hidden; ABI policy still needs a toolchain boundary.\n";
}
