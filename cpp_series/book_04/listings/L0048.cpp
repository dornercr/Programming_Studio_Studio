#include "check.hpp"
#include "fd.hpp"
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>

int main() {
    struct SimulatedFailure {};
    int released_number = -1;
    try {
        FileDescriptor original{::open("/dev/null", O_RDONLY | O_CLOEXEC)};
        CHECK(static_cast<bool>(original));
        released_number = original.get();
        FileDescriptor moved{std::move(original)};
        CHECK(!original && moved.get() == released_number);
        throw SimulatedFailure{};
    } catch (const SimulatedFailure&) {}
    errno = 0;
    CHECK(::fcntl(released_number, F_GETFD) == -1 && errno == EBADF);
    FileDescriptor file{::open("/dev/null", O_RDONLY | O_CLOEXEC)};
    CHECK(static_cast<bool>(file));
    file.close_checked();
    CHECK(!file);
    std::cout << "PASS\n";
}
