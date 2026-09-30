mkdir -p build
g++ --version
g++ -std=c++20 -Wall -Wextra -Wpedantic \
    source/ch01/main.cpp -o build/ch01
./build/ch01
