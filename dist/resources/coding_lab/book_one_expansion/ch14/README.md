# Chapter 14: Return a nullable borrowed selection

Return a pointer to the first element greater than threshold, or nullptr if none. Do not change or copy the vector. The pointer borrows its element and must not outlive or survive invalidation of that element.

## Design and rules
A pointer expresses both a live borrowed result and absence. The reference loop obtains addresses of actual vector elements rather than temporary copies.

A successful result points at the first strictly qualifying live element; nullptr represents no match.

Alternative: An optional index can avoid carrying an address across vector edits. It still requires a valid index and an unchanged intended selection meaning.

Starter defect: The starter selects the first 4 at threshold 4 instead of the later 9. Returning the address of a loop value copy would be a separate dangling-pointer bug.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch14/solution.cpp -o /tmp/book-one-ch14
printf '%s' '' | /tmp/book-one-ch14
```
Expected standard output:
```text
9
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
