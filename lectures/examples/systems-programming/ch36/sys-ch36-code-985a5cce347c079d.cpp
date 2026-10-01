#include <cstdio>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    const unsigned char data[3]{3, 5, 7};
    if (std::fwrite(data, 1, 3, file) != 3 || std::fflush(file) != 0) {
        std::fclose(file); return 1;
    }
    void* region = mmap(nullptr, 3, PROT_READ, MAP_PRIVATE, fileno(file), 0);
    const bool closed = std::fclose(file) == 0;
    if (region == MAP_FAILED) return 1;
    if (!closed) { munmap(region, 3); return 1; }
    const auto* bytes = static_cast<const unsigned char*>(region);
    unsigned sum = 0;
    for (unsigned i = 0; i < 3; ++i) sum += bytes[i];
    if (munmap(region, 3) != 0) return 1;
    std::cout << "mapped sum after close=" << sum << '\n';
}
