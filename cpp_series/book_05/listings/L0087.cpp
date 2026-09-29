#include "check.hpp"
#include <array>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>
struct Sample { std::uint32_t id; float value; std::array<char,48> note; };
class Columns {
    std::vector<std::uint32_t> ids_;
    std::vector<float> values_;
public:
    explicit Columns(std::span<const Sample> samples) {
        ids_.reserve(samples.size()); values_.reserve(samples.size());
        for (const auto& sample:samples) { ids_.push_back(sample.id); values_.push_back(sample.value); }
    }
    std::span<const float> values() const { return values_; }
    std::size_t size() const { return ids_.size(); }
};
double energy_records(std::span<const Sample> samples) {
    double result=0; for (const auto& sample:samples) result+=static_cast<double>(sample.value)*sample.value;
    return result;
}
double energy_columns(std::span<const float> values) {
    double result=0; for (float value:values) result+=static_cast<double>(value)*value;
    return result;
}
int main() {
    const std::array samples{Sample{1,2.0f,{}},Sample{2,-3.0f,{}},Sample{3,4.0f,{}}};
    const Columns columns{samples};
    CHECK(columns.size()==samples.size());
    CHECK(energy_records(samples)==29.0 && energy_columns(columns.values())==29.0);
    CHECK(energy_columns({})==0.0);
    std::cout << "record_bytes=" << sizeof(Sample) << " value_bytes=" << sizeof(float) << "\nPASS\n";
}
