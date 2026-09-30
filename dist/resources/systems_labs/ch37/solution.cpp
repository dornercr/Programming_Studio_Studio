#include <cassert>
#include <iostream>
#include <memory>

struct OpenFile { unsigned offset = 0; };

int main() {
    auto parent = std::make_shared<OpenFile>();
    auto child = parent;
    auto independent = std::make_shared<OpenFile>();
    child->offset += 2;
    assert(parent->offset == 2 && independent->offset == 0);
    child.reset();
    assert(parent && parent->offset == 2 && parent.use_count() == 1);
    std::cout << "parent-offset=" << parent->offset << " independent=" << independent->offset << '\n';
}
