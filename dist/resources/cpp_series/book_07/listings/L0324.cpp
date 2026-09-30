#include "harbor/net.hpp"
#include "harbor/wire.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <algorithm>
#include <array>
#include <cerrno>
#include <charconv>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>
namespace harbor {
namespace {
[[noreturn]] void os_error(const char* where) {
    throw std::system_error(errno, std::generic_category(), where);
}
void wait_for(int fd, short events, Steady::time_point until) {
    for (;;) {
        auto left=std::chrono::ceil<std::chrono::milliseconds>(until-Steady::now()).count();
        if (left<=0) throw std::runtime_error("RPC deadline");
        pollfd p{fd,events,0};
        int rc=::poll(&p,1,static_cast<int>(std::min<decltype(left)>(left,1000)));
        if (rc<0) { if (errno==EINTR) continue; os_error("poll"); }
        if (rc==0) continue;
        if (p.revents&POLLNVAL) throw std::runtime_error("invalid descriptor");
        // HUP/ERR must be interpreted by recv/send/getsockopt, not spun on.
        if (p.revents&(events|POLLERR|POLLHUP)) return;
    }
}
Fd connect_to(std::uint16_t port, Steady::time_point until) {
    Fd fd(::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0));
    if (fd.get()<0) os_error("socket");
    sockaddr_in address{}; address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_LOOPBACK); address.sin_port=htons(port);
    if (::connect(fd.get(),reinterpret_cast<sockaddr*>(&address),sizeof address)<0) {
        if (errno!=EINPROGRESS) os_error("connect");
        wait_for(fd.get(),POLLOUT,until);
        int error=0; socklen_t length=sizeof error;
        if (::getsockopt(fd.get(),SOL_SOCKET,SO_ERROR,&error,&length)<0) os_error("getsockopt");
        if (error) throw std::system_error(error,std::generic_category(),"connect completion");
    }
    return fd;
}
}
Fd::~Fd() { reset(); }
Fd::Fd(Fd&& other) noexcept: value_(other.release()) {}
Fd& Fd::operator=(Fd&& other) noexcept {
    if (this!=&other) reset(other.release());
    return *this;
}
int Fd::release() noexcept { return std::exchange(value_,-1); }
void Fd::reset(int value) noexcept {
    if (value_>=0) ::close(value_); // Linux close policy: do not retry EINTR
    value_=value;
}
std::uint16_t port_number(std::string_view text, bool allow_zero) {
    unsigned value=0;
    auto [end,ec]=std::from_chars(text.data(),text.data()+text.size(),value);
    if (ec!=std::errc{} || end!=text.data()+text.size() || value>65535 || (!value && !allow_zero))
        throw std::invalid_argument("invalid port");
    return static_cast<std::uint16_t>(value);
}
std::string exchange(std::uint16_t port, std::string_view request,
                     std::chrono::milliseconds timeout) {
    if (timeout.count()<=0) throw std::invalid_argument("timeout must be positive");
    auto until=Steady::now()+timeout;
    auto fd=connect_to(port,until);
    auto bytes=frame(request); std::size_t offset=0;
    while (offset<bytes.size()) {
        wait_for(fd.get(),POLLOUT,until);
        auto n=::send(fd.get(),bytes.data()+offset,bytes.size()-offset,MSG_NOSIGNAL);
        if (n>0) offset+=static_cast<std::size_t>(n);
        else if (n<0 && (errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK)) continue;
        else if (n<0) os_error("send");
        else throw std::runtime_error("zero send");
    }
    Decoder decoder;
    std::array<char,4100> buffer{};
    while (!decoder.ready()) {
        wait_for(fd.get(),POLLIN,until);
        auto n=::recv(fd.get(),buffer.data(),buffer.size(),0);
        if (n>0) decoder.feed({buffer.data(),static_cast<std::size_t>(n)});
        else if (n==0) throw std::runtime_error("EOF before reply");
        else if (errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK) continue;
        else os_error("recv");
    }
    return std::string(decoder.payload());
}
Server::Server(std::uint16_t port) {
    listener_.reset(::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0));
    if (listener_.get()<0) os_error("listener socket");
    int yes=1;
    if (::setsockopt(listener_.get(),SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes)<0) os_error("reuseaddr");
    sockaddr_in a{}; a.sin_family=AF_INET; a.sin_port=htons(port);
    a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if (::bind(listener_.get(),reinterpret_cast<sockaddr*>(&a),sizeof a)<0) os_error("bind");
    if (::listen(listener_.get(),64)<0) os_error("listen");
    socklen_t size=sizeof a;
    if (::getsockname(listener_.get(),reinterpret_cast<sockaddr*>(&a),&size)<0) os_error("getsockname");
    port_=ntohs(a.sin_port);
}
void Server::run(const Handler& handler, const std::function<bool()>& stopping) {
    struct Peer {
        Fd fd; Decoder decoder; std::string output; std::size_t offset=0;
        bool writing=false;
        Steady::time_point deadline=Steady::now()+std::chrono::seconds(2);
    };
    std::vector<Peer> peers;
    bool draining=false;
    for (;;) {
        draining=draining || stopping();
        if (draining && peers.empty()) return;
        std::vector<pollfd> ready{{draining ? -1 : listener_.get(),POLLIN,0}};
        for (const auto& p: peers) ready.push_back({p.fd.get(),static_cast<short>(p.writing?POLLOUT:POLLIN),0});
        int rc=::poll(ready.data(),ready.size(),25);
        if (rc<0) { if (errno==EINTR) continue; os_error("server poll"); }
        for (std::size_t i=0; i<peers.size(); ++i) {
            auto& p=peers[i]; auto re=ready[i+1].revents;
            if (Steady::now()>=p.deadline || (re&POLLNVAL)) { p.fd.reset(); continue; }
            try {
                if (!p.writing && (re&(POLLIN|POLLHUP|POLLERR))) {
                    std::array<char,4100> buf{};
                    auto n=::recv(p.fd.get(),buf.data(),buf.size(),0);
                    if (n>0) {
                        p.decoder.feed({buf.data(),static_cast<std::size_t>(n)});
                        if (p.decoder.ready()) {
                            auto reply=handler(p.decoder.payload());
                            if (!reply) { p.fd.reset(); continue; }
                            p.output=frame(*reply); p.writing=true;
                        }
                    } else if (n==0) p.fd.reset();
                    else if (errno!=EINTR && errno!=EAGAIN && errno!=EWOULDBLOCK) p.fd.reset();
                } else if (p.writing && (re&(POLLOUT|POLLERR|POLLHUP))) {
                    auto n=::send(p.fd.get(),p.output.data()+p.offset,p.output.size()-p.offset,MSG_NOSIGNAL);
                    if (n>0) {
                        p.offset+=static_cast<std::size_t>(n);
                        if (p.offset==p.output.size()) p.fd.reset();
                    } else if (n==0 || (errno!=EINTR && errno!=EAGAIN && errno!=EWOULDBLOCK)) p.fd.reset();
                }
            } catch (const std::exception&) { p.fd.reset(); }
        }
        std::erase_if(peers,[](const Peer& p){ return p.fd.get()<0; });
        if (!draining && (ready[0].revents&POLLIN)) {
            // At most 16 accepts per iteration avoids starving existing connections.
            for (int i=0; i<16; ++i) {
                Fd fd(::accept4(listener_.get(),nullptr,nullptr,SOCK_NONBLOCK|SOCK_CLOEXEC));
                if (fd.get()<0) {
                    if (errno==EAGAIN || errno==EWOULDBLOCK || errno==EINTR) break;
                    os_error("accept");
                }
                if (peers.size()<64) peers.push_back(Peer{std::move(fd),Decoder{}, {},0,false,
                    Steady::now()+std::chrono::seconds(2)});
                // Otherwise RAII closes the unadmitted socket, before parsing/allocation.
            }
        }
    }
}
}
