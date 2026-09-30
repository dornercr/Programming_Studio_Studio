#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstddef>
#include <iostream>

int main() {
    int fd = ::open("mapped.bin", O_RDONLY);
    if (fd < 0) return 1;
    struct stat st{}; if (::fstat(fd, &st) != 0 || st.st_size == 0) return 2;
    auto* p = static_cast<const unsigned char*>(::mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (p == MAP_FAILED) return 3;
    unsigned long long sum=0; for (off_t i=0;i<st.st_size;++i) sum += p[i];
    ::munmap(const_cast<unsigned char*>(p), st.st_size); ::close(fd);
    std::cout << sum << "\n";
}
