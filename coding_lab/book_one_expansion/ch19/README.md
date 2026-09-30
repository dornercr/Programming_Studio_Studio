# Chapter 19: Keep a declaration and definition in agreement

Declare subtotal inside namespace billing, then supply its matching definition. unit and count are 0 through 100. Return their product. Invalid input throws std::invalid_argument("range"). Keep the driver outside that namespace.

## Design and rules
A namespace-qualified definition visibly matches the declaration. The supplied offline split files demonstrate separate compilation of this same interface.

The public declaration and definition agree in namespace and parameter types; every accepted result equals unit times count.

Alternative: A header-only inline definition can fit small utilities. A separately compiled implementation is helpful when clients should depend only on the interface.

Starter defect: The starter links successfully but returns 11 instead of 28. Link success proves a definition exists, not that it implements the contract.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch19/solution.cpp -o /tmp/book-one-ch19
printf '%s' '7 4
' | /tmp/book-one-ch19
```
Expected standard output:
```text
28
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
