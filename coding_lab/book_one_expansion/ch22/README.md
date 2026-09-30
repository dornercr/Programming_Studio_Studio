# Chapter 22: Transfer units as one transaction

Initial counts are 0 through 1000. Reject bad indices, negative amount, insufficient source, or a destination above 1000. A same-index transfer succeeds without change when the amount is available. Any rejection leaves stock unchanged.

## Design and rules
Complete prevalidation works because the subsequent bounded int updates cannot fail. Same-object aliasing is handled before the two writes.

A successful distinct-index transfer preserves total units. Rejection and same-index success preserve all counts.

Alternative: A candidate copy works for more complex transactions, but these two bounded, nonthrowing writes need only a complete precheck.

Starter defect: The starter subtracts two from the source before discovering a full destination. It rejects with {8,999}, losing two units.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch22/solution.cpp -o /tmp/book-one-ch22
printf '%s' '' | /tmp/book-one-ch22
```
Expected standard output:
```text
false 10 999
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
