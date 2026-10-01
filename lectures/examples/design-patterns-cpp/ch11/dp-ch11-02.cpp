#include <iostream>
#include <string>
struct Mark {
    std::string name;
    int size;
    int x;
};
void draw(const Mark& mark) {
    std::cout << mark.name << '/' << mark.size << '@' << mark.x << '\n';
}
int main() {
    Mark left{"stamp", 12, 3};
    Mark right{"stamp", 12, 8};
    draw(left);
    draw(right);
    // These styles are values, so the change is local.
    left.size = 20;
    draw(left);
    draw(right);
}
