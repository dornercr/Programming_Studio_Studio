#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

class UniqueFd {
    int fd_{-1};
public:
    explicit UniqueFd(int fd) : fd_(fd) {}
    ~UniqueFd() { if (fd_ >= 0) ::close(fd_); }
    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;
    int get() const { return fd_; }
};

int main() {
    UniqueFd fd(::open("raii.txt", O_CREAT|O_WRONLY, 0600));
    if (fd.get() < 0) return 1;
    throw std::runtime_error("simulated failure");
}
