# Compute a checked row-major index

Map a row and column into a flat vector with a known number of columns. Check dimensions and bounds before accessing the data.

## Acceptance checks

- The (1,2) element of a 2×3 array is 6.
- Rows and columns outside the shape are rejected.
- The data length must match the declared shape.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch06/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The logical shape is part of the indexing contract; a flat allocation alone cannot prove coordinates are valid. Once the shape and coordinates are checked, row*cols+col selects the row-major element. Multiplication is bounded by data.size() before it is used. The vector owns the storage while the const reference only borrows it.

## Bug to diagnose

Use row+col instead of row*cols+col. Coordinate (1,2) then reads 4 instead of 6.

Count complete rows before adding the position inside the selected row. Test a non-square grid so swapped dimensions do not hide the error.
