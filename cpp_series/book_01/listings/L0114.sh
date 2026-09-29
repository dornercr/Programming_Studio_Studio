# From source/ch19 in a disposable build exercise:
g++ -std=c++20 -Wall -Wextra -Wpedantic \
  -g -c main.cpp -o main.o
g++ -std=c++20 -Wall -Wextra -Wpedantic \
  -g -c groups.cpp -o groups.o
g++ main.o groups.o -o groups_demo
./groups_demo
