g++ -std=c++20 -O0 -g compute.cpp -o compute_O0
g++ -std=c++20 -O3 -DNDEBUG compute.cpp -o compute_O3
size compute_O0 compute_O3
/usr/bin/time -f '%e s' ./compute_O0
/usr/bin/time -f '%e s' ./compute_O3
