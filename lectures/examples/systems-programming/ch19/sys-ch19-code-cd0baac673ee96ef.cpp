#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
int main() {
    const int fd = ::open("/dev/null", O_RDONLY);
    if (fd < 0) return 1;
    char byte = 'x';
    const auto read_result = ::read(fd, &byte, 1);
    const auto write_result = ::write(fd, &byte, 1);
    const int write_error = errno; // Save before another library call.
    const int closed = ::close(fd);
    if (read_result != 0 || write_result != -1 || write_error != EBADF || closed != 0) return 1;
    std::cout << std::boolalpha << "read-eof=" << (read_result == 0)
              << " write-denied=" << (write_error == EBADF) << '\n';
}
