set -eu
g++ -std=c++20 -c client.cpp -o client.o
g++ -std=c++20 client.o v1.cpp -o one
g++ -std=c++20 client.o v2.cpp -o two
test "$(./one)" = 7
test "$(./two)" = 7
printf 'unchanged client object: v1=7 v2=7\n'
