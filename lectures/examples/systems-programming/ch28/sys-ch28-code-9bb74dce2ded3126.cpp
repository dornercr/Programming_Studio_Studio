#include <iostream>
#include <set>
int main() {
    constexpr unsigned elements = 8, element_bytes = 4, line_bytes = 16;
    for (unsigned stride : {1u,4u}) {
        std::set<unsigned> blocks;
        for (unsigned i=0; i<elements; ++i)
            blocks.insert((i*stride*element_bytes)/line_bytes);
        std::cout << "stride=" << stride << " lines=" << blocks.size()
                  << " useful=" << elements*element_bytes
                  << " fetched=" << blocks.size()*line_bytes << '\n';
    }
}
