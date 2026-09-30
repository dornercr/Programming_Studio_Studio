# math.cpp -> math.o
g++ -std=c++20 -Wall -Wextra -c math.cpp -o math.o
# main.cpp -> main.o
g++ -std=c++20 -Wall -Wextra -c main.cpp -o main.o
# object files -> executable
g++ main.o math.o -o calculator
./calculator
