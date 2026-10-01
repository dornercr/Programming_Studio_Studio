#include <iostream>
struct Part { virtual ~Part() = default; };
struct Bolts final : Part {};
void report(const Part&) { std::cout << "Part overload\n"; }
void report(const Bolts&) { std::cout << "Bolts overload\n"; }
int main() {
    Bolts bolts;
    const Part& as_part = bolts; // Borrows the same complete object.
    report(bolts);
    report(as_part);
    std::cout << "same object: " << std::boolalpha
              << (&as_part == static_cast<const Part*>(&bolts)) << '\n';
}
