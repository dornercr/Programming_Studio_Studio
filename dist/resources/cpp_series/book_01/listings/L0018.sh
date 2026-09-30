mkdir -p cpp-project/src cpp-project/build
cd cpp-project
code .
g++ -std=c++20 -Wall -Wextra -Wpedantic -g src/main.cpp -o build/app
./build/app
