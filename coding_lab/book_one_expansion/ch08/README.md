# Chapter 8: Separate a price calculation from its caller

Return unit times count when unit is 0 through 1000 and count 0 through 100. Otherwise throw std::invalid_argument("range"). Do not print inside the function.

## Design and rules
A value-returning helper isolates the calculation. A long intermediate and range check make conversion to int deliberate.

The helper prints nothing; accepted arguments produce their mathematical product if representable, and invalid inputs are rejected.

Alternative: Reading and printing inside the helper can work for a one-off demo, but couples pricing to a terminal and makes reuse harder.

Starter defect: The starter returns 15 for unit 12 and count 3 instead of 36. Its calculation violates the pricing requirement.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch08/solution.cpp -o /tmp/book-one-ch08
printf '%s' '12 3
' | /tmp/book-one-ch08
```
Expected standard output:
```text
36
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
