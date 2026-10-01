#include <cassert>
#include <iostream>

struct Entry { bool present; bool readable; bool writable; };
enum class Access { Read, Write };
bool allows(const Entry& entry, Access access) {
    return entry.present && (access == Access::Write ? entry.writable : entry.readable);
}

int main() {
    const Entry read_only{true,true,false}, read_write{true,true,true}, absent{false,true,true};
    assert(allows(read_only,Access::Read) && !allows(read_only,Access::Write));
    assert(allows(read_write,Access::Read) && allows(read_write,Access::Write));
    assert(!allows(absent,Access::Read) && !allows(absent,Access::Write));
    std::cout << "read-only: read=yes write=no; absent: denied\n";
}
