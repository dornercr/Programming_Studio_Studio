# Chapter 6: Partition score boundaries

Scores 0 through 100 map to high at 80 or above, pass at 50 through 79, and retry below 50. Outside 0 through 100 return invalid.

## Design and rules
An ordered exclusive chain gives each valid score exactly one label. The order reflects nested threshold ranges.

Each boundary belongs to one band; invalid input never receives a successful score band.

Alternative: Explicit disjoint ranges are also valid, but repeating every upper bound creates more places for a later policy edit to drift.

Starter defect: The starter returns pass for 80 because it returns from the broader >=50 branch before reaching the high test.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch06/solution.cpp -o /tmp/book-one-ch06
printf '%s' '80
' | /tmp/book-one-ch06
```
Expected standard output:
```text
high
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
