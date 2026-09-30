# Chapter 7: Sum accepted readings before a sentinel

Read in order. Stop at -999 or after accepting three values. Ignore other negatives. Accepted values are 0 through 100; input obeys these value categories. Return accepted count and sum.

## Design and rules
A range loop advances safely even on continue. Separate count and sum track the same accepted prefix.

The count is at most three, and the sum describes exactly those count accepted readings before the sentinel.

Alternative: An index loop is useful when positions matter. Here it adds a manual index without helping the acceptance rule.

Starter defect: The starter accepts four values and returns (4,13) for the sample, violating the three-item boundary.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch07/solution.cpp -o /tmp/book-one-ch07
printf '%s' '1 -2 3 4 5
' | /tmp/book-one-ch07
```
Expected standard output:
```text
3 8
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
