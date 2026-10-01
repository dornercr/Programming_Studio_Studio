#include <cstdio>
#include <iostream>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    const unsigned char source[6]{'A','B',0,'C','D','E'};
    unsigned char result[6]{};
    const auto written = std::fwrite(source, 2, 3, file);
    bool okay = written == 3 && std::fseek(file, 0, SEEK_SET) == 0;
    const auto received = okay ? std::fread(result, 1, 6, file) : 0;
    okay = received == 6 && !std::ferror(file) && okay;
    okay = std::fclose(file) == 0 && okay;
    if (!okay) return 1;
    std::cout << "written-elements=" << written << '\n';
    std::cout << "read-bytes=" << received << '\n';
    std::cout << "embedded-zero=" << static_cast<unsigned>(result[2]) << '\n';
}
