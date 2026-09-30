#include <iostream>
int next_id() {
    static int id = 0;
    return ++id;
}
int main(){ std::cout << next_id() << ' ' << next_id() << '\n'; }
