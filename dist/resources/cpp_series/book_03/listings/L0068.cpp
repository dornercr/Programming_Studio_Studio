#include "course_test.hpp"
#include <algorithm>
#include <functional>
#include <random>
#include <vector>
#include <utility>

void insertion_sort(std::vector<int>& data) {
    for (std::size_t i = 1; i < data.size(); ++i) {
        const int value = data[i];
        std::size_t j = i;
        while (j > 0 && value < data[j - 1]) { data[j] = data[j - 1]; --j; }
        data[j] = value;
    }
}
void selection_sort(std::vector<int>& data) {
    for (std::size_t i = 0; i < data.size(); ++i) {
        auto smallest = i;
        for (auto j = i + 1; j < data.size(); ++j) if (data[j] < data[smallest]) smallest = j;
        std::swap(data[i], data[smallest]);
    }
}
void merge_range(std::vector<int>& data, std::vector<int>& temp,
                 std::size_t low, std::size_t high) {
    if (high - low <= 1) return;
    const auto middle = low + (high - low) / 2;
    merge_range(data, temp, low, middle); merge_range(data, temp, middle, high);
    auto left = low, right = middle;
    for (auto out = low; out < high; ++out) {
        if (left < middle && (right == high || !(data[right] < data[left]))) {
            temp[out] = data[left++];
        } else { temp[out] = data[right++]; }
    }
    for (auto i = low; i < high; ++i) data[i] = temp[i];
}
void merge_sort(std::vector<int>& data) {
    std::vector<int> temporary(data.size());
    merge_range(data, temporary, 0, data.size());
}
void quick_range(std::vector<int>& data, std::size_t low, std::size_t high) {
    while (high - low > 1) {
        const int pivot = data[low + (high - low) / 2];
        auto less = low, current = low, greater = high;
        while (current < greater) {
            if (data[current] < pivot) std::swap(data[less++], data[current++]);
            else if (pivot < data[current]) std::swap(data[current], data[--greater]);
            else ++current;
        }
        // Recurse on the smaller side; iterate on the larger side.
        if (less - low < high - greater) {
            quick_range(data, low, less); low = greater;
        } else {
            quick_range(data, greater, high); high = less;
        }
    }
}
void quick_sort(std::vector<int>& data) { quick_range(data, 0, data.size()); }

int main() {
    std::mt19937 random{42};
    const std::vector<std::function<void(std::vector<int>&)>> algorithms{
        insertion_sort, selection_sort, merge_sort, quick_sort};
    for (int trial = 0; trial < 200; ++trial) {
        std::vector<int> input(static_cast<std::size_t>(trial) % 129);
        for (int& value : input) value = static_cast<int>(random() % 31) - 15;
        if (trial % 4 == 0) std::sort(input.begin(), input.end());
        if (trial % 4 == 1) std::sort(input.rbegin(), input.rend());
        if (trial % 4 == 2) std::fill(input.begin(), input.end(), 7);
        auto expected = input; std::sort(expected.begin(), expected.end());
        for (const auto& algorithm : algorithms) {
            auto actual = input; algorithm(actual); CHECK(actual == expected);
        }
    }
    course::report();
}
