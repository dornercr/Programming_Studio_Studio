#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>
int main() {
    char name[] = "/tmp/studio-map-XXXXXX";
    const int fd = mkstemp(name);
    if (fd < 0) throw std::runtime_error("mkstemp");
    if (unlink(name) != 0 || ftruncate(fd,1) != 0)
        throw std::runtime_error("prepare file");
    void* first = mmap(nullptr,1,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (first == MAP_FAILED) throw std::runtime_error("first mmap");
    void* second = mmap(nullptr,1,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (second == MAP_FAILED) throw std::runtime_error("second mmap");
    auto* a = static_cast<char*>(first);
    auto* b = static_cast<char*>(second);
    *a = 'Q';
    std::cout << "distinct-views=" << std::boolalpha << (a != b)
              << " shared-byte=" << *b << '\n';
    if (munmap(first,1) != 0 || munmap(second,1) != 0 || close(fd) != 0)
        throw std::runtime_error("cleanup");
}
