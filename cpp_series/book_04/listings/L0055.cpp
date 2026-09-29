#include <fcntl.h>
#include <unistd.h>
#include <array>
#include <iostream>

int main() {
    int fd = ::open("bytes.bin", O_CREAT|O_TRUNC|O_RDWR, 0600);
    if (fd < 0) return 1;
    std::array<unsigned char,4> out{1,2,3,4}, in{};
    if (::write(fd, out.data(), out.size()) != 4) return 2;
    ::lseek(fd, 0, SEEK_SET);
    if (::read(fd, in.data(), in.size()) != 4) return 3;
    ::close(fd);
    std::cout << +in[0] << ' ' << +in[3] << "\n";
}
