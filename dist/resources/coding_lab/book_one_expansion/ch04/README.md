# Chapter 4: Increment without signed overflow

Return value plus one when representable as int. At the maximum int throw std::out_of_range("overflow") before arithmetic.

## Design and rules
Checking numeric_limits makes the representation boundary explicit before addition. The function returns a value and does not mutate caller state.

No path evaluates overflowing signed addition; accepted values increase by exactly one.

Alternative: A wider type can help when its range is known to cover the calculation. It still needs a checked conversion to int afterward.

Starter defect: The starter returns 9 for input 9 instead of 10. An unchecked value+1 at the maximum would have undefined behavior, not a promised wrapped output.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch04/solution.cpp -o /tmp/book-one-ch04
printf '%s' '9
' | /tmp/book-one-ch04
```
Expected standard output:
```text
10
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
