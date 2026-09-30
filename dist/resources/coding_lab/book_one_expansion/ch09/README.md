# Chapter 9: Keep counters independent

counter must be 0 through 999. Return its current value and increment the same caller-owned counter. On invalid input throw std::invalid_argument("range") without changing it.

## Design and rules
A reference makes the changing state explicit at the call boundary. Separate callers can maintain independent sequences.

Success issues the old count and advances only that counter; rejection changes no state.

Alternative: A function-local static hides one persistent sequence shared by callers. It fits a deliberately global service, but weakens independent tests here.

Starter defect: The starter leaves counter at 7 after issuing ticket 7. The next call would issue the same ticket again.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch09/solution.cpp -o /tmp/book-one-ch09
printf '%s' '7
' | /tmp/book-one-ch09
```
Expected standard output:
```text
7 8
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
