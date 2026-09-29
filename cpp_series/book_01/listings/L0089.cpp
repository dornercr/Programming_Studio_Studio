#include <iostream>
struct Item{ int id; };
void print(const Item* item){
    if(item) std::cout << item->id << '\n';
    else std::cout << "none\n";
}
int main(){ Item x{7}; print(&x); print(nullptr); }
