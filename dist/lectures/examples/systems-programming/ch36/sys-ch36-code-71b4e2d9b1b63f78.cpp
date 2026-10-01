#include <cstdio>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    const unsigned char initial = 4;
    if (std::fwrite(&initial, 1, 1, file) != 1 || std::fflush(file) != 0) {
        std::fclose(file); return 1;
    }
    void* region = mmap(nullptr, 1, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE, fileno(file), 0);
    if (region == MAP_FAILED) { std::fclose(file); return 1; }
    auto* byte = static_cast<unsigned char*>(region);
    *byte = 9;
    const unsigned observed = *byte;
    unsigned char backing = 0;
    bool okay = pread(fileno(file), &backing, 1, 0) == 1;
    okay = munmap(region, 1) == 0 && okay;
    okay = std::fclose(file) == 0 && okay;
    if (!okay) return 1;
    std::cout << "private=" << observed << " backing="
              << static_cast<unsigned>(backing) << '\n';
}
