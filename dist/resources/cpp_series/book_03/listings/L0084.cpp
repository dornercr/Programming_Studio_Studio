#include "course_test.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

using Sum = std::uint64_t;
Sum row_major(const std::vector<unsigned>& data, std::size_t width) {
    Sum sum{};
    for (std::size_t row = 0; row < width; ++row)
        for (std::size_t col = 0; col < width; ++col) sum += data[row * width + col];
    return sum;
}
Sum column_major(const std::vector<unsigned>& data, std::size_t width) {
    Sum sum{};
    for (std::size_t col = 0; col < width; ++col)
        for (std::size_t row = 0; row < width; ++row) sum += data[row * width + col];
    return sum;
}
struct Sample { float x, y, z; };

template<class Function>
long long trial(Function function, const std::vector<unsigned>& data, std::size_t width,
                Sum& result) {
    const auto start = std::chrono::steady_clock::now();
    result = function(data, width);
    const auto stop = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}

int main(int argc, char** argv) {
    const bool benchmark = argc == 2 && std::string_view{argv[1]} == "--benchmark";
    const std::size_t width = benchmark ? 1024 : 32;
    std::vector<unsigned> matrix(width * width);
    for (std::size_t i = 0; i < matrix.size(); ++i) matrix[i] = static_cast<unsigned>(i % 100);
    CHECK(row_major(matrix, width) == column_major(matrix, width));
    const std::vector<Sample> aos{{1, 2, 3}, {4, 5, 6}};
    const std::vector<float> x{1, 4}, y{2, 5}, z{3, 6};
    CHECK(aos[0].x + aos[1].x == x[0] + x[1]);
    CHECK(y.size() == z.size() && z.size() == x.size());
    if (benchmark) {
        std::vector<long long> row_times, column_times;
        Sum checksum{};
        for (int i = 0; i < 9; ++i) {
            matrix[0] = static_cast<unsigned>(i);
            Sum row{}, column{};
            // Alternate order to reduce a simple first/second measurement bias.
            if (i % 2 == 0) {
                row_times.push_back(trial(row_major, matrix, width, row));
                column_times.push_back(trial(column_major, matrix, width, column));
            } else {
                column_times.push_back(trial(column_major, matrix, width, column));
                row_times.push_back(trial(row_major, matrix, width, row));
            }
            CHECK(row == column); checksum += row;
        }
        std::sort(row_times.begin(), row_times.end());
        std::sort(column_times.begin(), column_times.end());
        std::cout << "row_median_ns=" << row_times[4]
                  << " column_median_ns=" << column_times[4]
                  << " checksum=" << checksum << '\n';
    }
    course::report();
}
