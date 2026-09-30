# Chapter 10: Respect both array dimensions

Return two totals, one per row of the fixed 2-by-3 grid. Each element is -100 through 100. Preserve the input and include every column.

## Design and rules
Nested range loops follow the actual row and column extents. A local sum is reset for each row.

Each result sums exactly three elements of its matching row, and the borrowed grid is unchanged.

Alternative: Index loops are useful when a computation needs coordinates. For plain summation, manual bounds add an avoidable maintenance risk.

Starter defect: The starter returns [3,9] for the sample because it drops each third column.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch10/solution.cpp -o /tmp/book-one-ch10
printf '%s' '1 2 3 4 5 6
' | /tmp/book-one-ch10
```
Expected standard output:
```text
6 15
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
