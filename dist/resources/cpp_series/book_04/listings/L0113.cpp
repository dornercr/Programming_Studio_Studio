#include "native.hpp"
#include "check.hpp"
#include <array>
#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>

namespace {
void transfer(harbor::Fd& target, harbor::Fd&& source) { target = std::move(source); }
}
int main() {
    try {
        std::array<char, 26> name{};
        const std::string pattern = "/tmp/harbor-fd-XXXXXX";
        std::copy(pattern.begin(), pattern.end(), name.begin());
        const int raw = ::mkstemp(name.data());
        if (raw < 0) throw std::runtime_error("mkstemp");
        harbor::Fd first(raw);
        if (::unlink(name.data()) < 0) throw std::runtime_error("unlink owned scratch file");
        CHECK(::fcntl(first.get(), F_SETFD, FD_CLOEXEC) == 0);
        CHECK(::write(first.get(), "abc", 3) == 3); // small regular-file fixture
        CHECK(::lseek(first.get(), 0, SEEK_SET) == 0);
        const int copied = ::dup(first.get());
        if (copied < 0) throw std::runtime_error("dup");
        harbor::Fd second(copied);
        CHECK(first.get() != second.get());
        CHECK((::fcntl(first.get(), F_GETFD) & FD_CLOEXEC) != 0);
        CHECK((::fcntl(second.get(), F_GETFD) & FD_CLOEXEC) == 0);
        char a = 0, b = 0, c = 0;
        CHECK(::read(first.get(), &a, 1) == 1);
        CHECK(::read(second.get(), &b, 1) == 1);
        CHECK(a == 'a' && b == 'b'); // duplicated descriptors share offset
        errno = E2BIG;
        first.reset();
        CHECK(errno == E2BIG);
        CHECK(first.get() == -1);
        harbor::Fd moved(std::move(second));
        CHECK(second.get() == -1);
        CHECK(::read(moved.get(), &c, 1) == 1 && c == 'c');
        CHECK(::read(moved.get(), &c, 1) == 0);
        transfer(moved, std::move(moved)); // aliased self-move through the owner interface
        CHECK(moved.get() >= 0);
        std::cout << "PASS descriptor ownership, shared offset, and per-descriptor flags\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
