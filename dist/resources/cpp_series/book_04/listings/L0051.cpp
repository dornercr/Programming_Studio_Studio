#include <cstddef>
#include <iostream>
#include <new>

class Arena {
    alignas(std::max_align_t) std::byte storage_[128]{};
    std::size_t used_{};
public:
    void* allocate(std::size_t n) {
        if (used_ + n > sizeof storage_) throw std::bad_alloc{};
        void* p = storage_ + used_;
        used_ += n;
        return p;
    }
};

int main() {
    Arena a;
    using Int = int;
    auto* x = new (a.allocate(sizeof(Int))) Int(42);
    std::cout << *x << "\n";
    x->~Int();
}
