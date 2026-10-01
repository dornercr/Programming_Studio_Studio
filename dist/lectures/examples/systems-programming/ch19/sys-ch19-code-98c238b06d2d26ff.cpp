#include <iostream>
struct ProtectedObject { unsigned owner; };
bool authorized(unsigned caller, const ProtectedObject& object) {
    return caller == object.owner; // A deliberately narrow policy model.
}
int main() {
    const ProtectedObject object{17};
    std::cout << std::boolalpha
              << "owner=" << authorized(17, object)
              << " stranger=" << authorized(23, object) << '\n';
}
