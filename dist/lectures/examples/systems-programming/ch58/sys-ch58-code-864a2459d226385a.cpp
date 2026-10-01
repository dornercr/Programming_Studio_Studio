#include <cassert>
#include <iostream>

struct Guarded { int payload; unsigned guard; };
bool intact(const Guarded& value) { return value.guard == 0xA55Au; }

int main() {
    Guarded record{7,0xA55Au};
    assert(intact(record));
    record.payload = 99;
    assert(intact(record));
    const bool payload_only = intact(record);
    record.guard ^= 1u;
    assert(!intact(record));
    std::cout << "payload-only=" << payload_only << " guard-change=" << intact(record) << '\n';
}
