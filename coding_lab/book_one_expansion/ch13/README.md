# Chapter 13: Shift a collection without partial writes

Input elements and delta are -100 through 100. Shift every value by delta only if every result stays in -100 through 100. Otherwise return false with the whole vector unchanged.

## Design and rules
The first loop proves all integer results fit the domain. The second uses references to update actual elements. The stated small bounds keep addition representable.

Either all elements shift, or no element changes. Validation must complete before mutation begins.

Alternative: A candidate copy is useful when mutation can fail after validation. Here int assignment does not throw, so two passes avoid the extra copy.

Starter defect: The starter changes 1 to 3 before rejecting 99+2. It returns false with state {3,99}, violating all-or-nothing mutation.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch13/solution.cpp -o /tmp/book-one-ch13
printf '%s' '' | /tmp/book-one-ch13
```
Expected standard output:
```text
false 1 99
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
