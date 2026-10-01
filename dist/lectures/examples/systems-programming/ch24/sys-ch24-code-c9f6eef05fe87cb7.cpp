#include <iostream>
struct Entry { bool present; bool user; bool writable; };
bool user_access(const Entry& root, const Entry& leaf, bool write) {
    return root.present && leaf.present && root.user && leaf.user
        && (!write || (root.writable && leaf.writable));
}
int main() {
    const Entry root{true,true,false};
    const Entry leaf{true,true,true};
    std::cout << std::boolalpha;
    std::cout << "leaf-only-write=" << (leaf.present && leaf.user && leaf.writable) << '\n';
    std::cout << "combined-write=" << user_access(root,leaf,true) << '\n';
    std::cout << "combined-read=" << user_access(root,leaf,false) << '\n';
}
