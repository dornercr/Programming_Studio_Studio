g++ -std=c++20 -O0 -S -masm=intel optimize.cpp -o O0.s
g++ -std=c++20 -O3 -S -masm=intel optimize.cpp -o O3.s
diff -u O0.s O3.s | less
