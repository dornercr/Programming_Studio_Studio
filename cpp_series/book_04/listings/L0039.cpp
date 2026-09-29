#include <vector>

int main() {
    std::vector<int> values{1,2,3};
    volatile int x = values[3]; // intentional bug for sanitizer exercise
    (void)x;
}
