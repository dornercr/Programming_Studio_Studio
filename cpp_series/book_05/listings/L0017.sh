g++ -std=c++20 -c src/add.cpp -Iinclude -o add.o
ar rcs libcalc.a add.o
ar t libcalc.a
g++ -std=c++20 app/main.cpp -Iinclude libcalc.a -o calculator
