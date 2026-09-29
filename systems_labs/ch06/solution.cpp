#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stdexcept>

int at_grid(const std::vector<int>& data, std::size_t rows, std::size_t cols,
            std::size_t row, std::size_t col) {
    if(cols != 0 && rows > data.size()/cols) throw std::invalid_argument("shape");
    if(rows*cols != data.size()) throw std::invalid_argument("shape");
    if(row >= rows || col >= cols) throw std::out_of_range("coordinate");
    return data[row*cols+col];
}

int main() {
    const std::vector<int> data{1,2,3,4,5,6};
    assert(at_grid(data,2,3,1,2) == 6);
    int rejected = 0;
    try { at_grid(data,2,3,2,0); } catch(const std::out_of_range&) { ++rejected; }
    try { at_grid(data,2,3,0,3); } catch(const std::out_of_range&) { ++rejected; }
    try { at_grid(data,3,3,0,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 3);
    std::cout << "value=" << at_grid(data,2,3,1,2) << " rejected=" << rejected << '\n';
}
