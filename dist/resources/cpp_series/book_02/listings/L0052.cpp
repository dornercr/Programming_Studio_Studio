#include "course_test.hpp"
#include <array>
#include <cstddef>
#include <string_view>

template<class T, std::size_t Rows, std::size_t Columns>
class Matrix {
    static_assert(Rows > 0 && Columns > 0);
    static_assert(Columns == 0 || Rows <= static_cast<std::size_t>(-1) / Columns);
    std::array<T, Rows * Columns> data_{};
public:
    T& at(std::size_t row, std::size_t column) {
        if (row >= Rows || column >= Columns) throw std::out_of_range("matrix");
        return data_[row * Columns + column];
    }
};
template<class T> struct Category {
    static constexpr std::string_view name{"value"};
};
template<> struct Category<bool> {
    static constexpr std::string_view name{"boolean"};
};
template<class T> struct Category<T*> {
    static constexpr std::string_view name{"pointer"};
};

int main() {
    Matrix<int, 2, 3> matrix;
    matrix.at(1, 2) = 7;
    CHECK(matrix.at(1, 2) == 7 && matrix.at(0, 0) == 0);
    CHECK(course::throws<std::out_of_range>([&] { (void)matrix.at(2, 0); }));
    static_assert(Category<int>::name == "value");
    static_assert(Category<bool>::name == "boolean");
    static_assert(Category<int*>::name == "pointer");
    course::report();
}
