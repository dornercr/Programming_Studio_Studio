# Chapter 20: Commit inventory batches only after full validation

Define struct Change { std::size_t index; int delta; }. Initial stock values are 0 through 1000; deltas are -1000 through 1000. Apply changes in order to a candidate. Reject any bad index or intermediate count outside 0 through 1000, preserving original stock. Empty batches succeed.

## Design and rules
A candidate vector owns tentative state. The single commit separates validation from original-state mutation and handles interacting changes in order.

Success commits all changes; any rejection preserves every original element. Every intermediate candidate count must meet the domain.

Alternative: A carefully proved prevalidation pass can avoid a copy, but repeated indices and intermediate limits make that proof more involved here.

Starter defect: The starter changes stock[0] from 5 to 4 before rejecting the second change. It returns false with partial state {4,2}.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch20/solution.cpp -o /tmp/book-one-ch20
printf '%s' '' | /tmp/book-one-ch20
```
Expected standard output:
```text
false 5 2
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
