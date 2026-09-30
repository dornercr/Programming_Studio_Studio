#include "check.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory_resource>
#include <optional>
#include <string>
#include <vector>

struct Handle { std::size_t index; std::uint64_t generation; };
class StringPool {
    struct Slot { std::optional<std::string> value; std::uint64_t generation{}; };
    std::array<Slot, 2> slots_{};
public:
    std::optional<Handle> acquire(std::string value) {
        for (std::size_t i = 0; i < slots_.size(); ++i) {
            auto& slot = slots_[i];
            if (!slot.value && slot.generation != std::numeric_limits<std::uint64_t>::max()) {
                slot.value.emplace(std::move(value));
                return Handle{i, slot.generation};
            }
        }
        return std::nullopt;
    }
    const std::string* get(Handle handle) const {
        if (handle.index >= slots_.size()) return nullptr;
        const auto& slot = slots_[handle.index];
        return slot.value && slot.generation == handle.generation ? &*slot.value : nullptr;
    }
    bool release(Handle handle) {
        if (!get(handle)) return false;
        auto& slot = slots_[handle.index];
        slot.value.reset(); ++slot.generation;
        return true;
    }
};
int main() {
    alignas(std::max_align_t) std::array<std::byte, 2048> storage{};
    std::pmr::monotonic_buffer_resource arena{storage.data(), storage.size(), std::pmr::null_memory_resource()};
    {
        std::pmr::vector<int> values{&arena};
        values.reserve(128);
        for (int i = 0; i < 128; ++i) values.push_back(i);
        CHECK(values.back() == 127);
    }
    arena.release(); // All users of the previous phase have been destroyed.
    bool exhausted = false;
    try { (void)arena.allocate(8192, alignof(std::max_align_t)); }
    catch (const std::bad_alloc&) { exhausted = true; }
    CHECK(exhausted);
    StringPool pool;
    const auto first = pool.acquire("web1"); const auto second = pool.acquire("web2");
    CHECK(first && second && !pool.acquire("web3"));
    CHECK(*pool.get(*first) == "web1");
    CHECK(pool.release(*first) && !pool.get(*first));
    const auto replacement = pool.acquire("web3");
    CHECK(replacement && replacement->index == first->index);
    CHECK(!pool.get(*first) && *pool.get(*replacement) == "web3");
    std::cout << "PASS\n";
}
