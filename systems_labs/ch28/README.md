# Generalize the traversal trace

Count misses for a square row-major array in a one-line cache. The dimension and elements per line are inputs from 1 through 64; reject zero. Compare both traversal orders.

## Acceptance checks

- A 4×4 array with four elements per line gives 4 row-first and 16 column-first misses.
- A 1×1 array gives one miss for either order.
- Zero dimension or line length is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch28/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Both traversals touch exactly the same indexes but in different orders. A line contains consecutive element positions, so row traversal uses nearby values before replacing the resident line. The input bounds also keep index arithmetic small. This miss count is a model result, not a timing benchmark or a prediction for all real cache sizes.

## Bug to diagnose

Swap the loop order but keep the same index formula using outer as the row. The supposed column traversal is still row traversal.

Define what outer and inner mean under each order, then calculate the resulting flat index. Inspect the first four indexes before counting misses.
