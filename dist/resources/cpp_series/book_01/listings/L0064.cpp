#include <iostream>

int persistent_next() {
    static int value{};
    return ++value;
}

int local_next(int previous) {
    const int next = previous + 1;
    return next; // Return a value, not its address.
}

int main() {
    const int first = persistent_next();
    const int second = persistent_next();
    const int explicit_result = local_next(10);
    if (first != 1 || second != 2 || explicit_result != 11) return 1;
    {
        const int scoped = local_next(2);
        if (scoped != 3) return 2;
    }
    std::cout << "persistent=" << first << ',' << second << '\n';
    std::cout << "explicit=" << explicit_result << '\n';
    std::cout << "PASS\n";
}
