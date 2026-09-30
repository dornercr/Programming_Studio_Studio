# Chapter 5: Keep quotient and remainder consistent

items must be 0 through 1000 and group 1 through 100. Throw std::invalid_argument("range") otherwise. Return complete groups and leftover items.

## Design and rules
Integer quotient and remainder express complete groups without introducing floating-point rounding.

items equals groups times group plus remainder, and remainder is smaller than group.

Alternative: Repeated subtraction can produce the same values but does unnecessary work and adds a loop invariant to maintain.

Starter defect: The starter reports 2 groups and 3 leftovers for 17 items in groups of 5. Those results do not reconstruct the original 17 items.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch05/solution.cpp -o /tmp/book-one-ch05
printf '%s' '17 5
' | /tmp/book-one-ch05
```
Expected standard output:
```text
3 2
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
