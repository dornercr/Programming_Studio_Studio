set -eu
mkdir -p build stage
g++ -std=c++20 -Wall -Wextra -O2 main.cpp -o build/test
./build/test
cp build/test stage/verified-test
printf 'tests passed before artifact staging\n'
