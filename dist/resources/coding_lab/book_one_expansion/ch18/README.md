# Chapter 18: Establish a valid interval at construction

Interval(int low,int high) accepts -100 through 100 endpoints with low<=high. Reject invalid construction with std::invalid_argument("range"). low() const and high() const report the endpoints. A one-point interval is valid.

## Design and rules
The initializer list supplies member values, and the constructor rejects a relationship that cannot establish the invariant. No successfully constructed invalid interval escapes.

Every live successfully constructed Interval has -100<=low<=high<=100.

Alternative: A factory returning an explicit failure value is useful when rejected input is expected control flow. A public unvalidated pair leaves the invariant to every caller.

Starter defect: The starter throws for (3,3), even though equality denotes a valid one-point interval. A constructor must match the stated domain, not a guessed one.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch18/solution.cpp -o /tmp/book-one-ch18
printf '%s' '' | /tmp/book-one-ch18
```
Expected standard output:
```text
3 3
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
