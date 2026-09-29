g++ -std=c++20 -E main.cpp -o main.ii
g++ -std=c++20 -S main.ii -o main.s
g++ -c main.s -o main.o
g++ main.o -o app
./app
