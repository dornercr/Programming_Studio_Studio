# Decompose a cache address for variable geometry

Return block, set, and tag for nonzero line size and set count. Accept byte addresses as unsigned integers; reject zero geometry.

## Acceptance checks

- Address 64 with 16-byte lines and four sets gives block 4, set 0, tag 1.
- Addresses within the same line have the same block, set, and tag.
- A zero line size or set count is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch29/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Division removes the byte offset, modulo selects a set, and the remaining quotient distinguishes competing blocks. Powers of two allow bit-field extraction, but this numeric model also supports other positive sizes. Decomposition does not test whether the cache line is valid or resident; lookup needs that additional state.

## Bug to diagnose

Use address%sets directly for the set number. Byte offsets inside one cache line then appear to choose different sets.

Remove the line offset first by dividing by line_size, then compute the set from the block number.
