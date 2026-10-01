#include <iostream>
#include <stdexcept>
#include <unistd.h>
class Owner {
    int fd_;
    int& closed_;
public:
    Owner(int fd, int& closed) : fd_(fd), closed_(closed) {}
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    ~Owner() { if (::close(fd_) == 0) ++closed_; }
};
int main() {
    int ends[2];
    if (::pipe(ends) != 0) return 1;
    int closed = 0;
    bool caught = false;
    try {
        Owner read_end(ends[0], closed), write_end(ends[1], closed);
        throw std::runtime_error("later setup failed");
    } catch (const std::runtime_error&) { caught = true; }
    std::cout << "closed=" << closed << " caught=" << caught << '\n';
    return closed == 2 && caught ? 0 : 1;
}
