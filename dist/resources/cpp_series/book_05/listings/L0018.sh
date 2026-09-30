g++ -std=c++20 -fPIC -fvisibility=hidden -c src/add.cpp -Iinclude -o add.o
g++ -shared add.o -o libcalc.so
nm -D --defined-only libcalc.so
