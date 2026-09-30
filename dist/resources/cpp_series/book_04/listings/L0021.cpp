#include "check.hpp"
#include <cstddef>
#include <memory_resource>
#include <vector>

class CountingResource : public std::pmr::memory_resource {
public:
    std::size_t allocations{}, deallocations{}, live_bytes{}, peak_bytes{};
private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* block = std::pmr::new_delete_resource()->allocate(bytes, alignment);
        ++allocations;
        live_bytes += bytes;
        if (live_bytes > peak_bytes) peak_bytes = live_bytes;
        return block;
    }
    void do_deallocate(void* block, std::size_t bytes, std::size_t alignment) override {
        std::pmr::new_delete_resource()->deallocate(block, bytes, alignment);
        ++deallocations;
        live_bytes -= bytes;
    }
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
};
int call_chain(int depth) {
    const int local = depth;
    if (depth == 0) return local;
    return local + call_chain(depth - 1);
}
int main() {
    CHECK(call_chain(4) == 10);
    CountingResource resource;
    {
        std::pmr::vector<int> values{&resource};
        values.reserve(128);
        for (int i = 0; i < 128; ++i) values.push_back(i);
        CHECK(resource.allocations >= 1);
        CHECK(resource.live_bytes >= values.capacity() * sizeof(int));
        CHECK(values.back() == 127);
    }
    CHECK(resource.live_bytes == 0);
    CHECK(resource.allocations == resource.deallocations);
    std::cout << "resource_requests=" << resource.allocations
              << " peak_requested_bytes=" << resource.peak_bytes << "\nPASS\n";
}
