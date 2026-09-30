# Check an effective-address calculation

Compute base+index*scale+displacement under a caller-supplied upper bound. The result is a numeric model, not a dereferenceable C++ pointer.

## Acceptance checks

- 1000 + 3×4 + 8 is 1020 under limit 2000.
- Reject a scaled index or displacement that exceeds remaining range.
- Zero index works without requiring a transfer of data.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch14/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The model separates computing a value from accessing memory. Every addition is preceded by a remaining-range check, and multiplication is bounded before it occurs. A real addressing mode has its own width and validity rules. Passing these numeric checks alone does not create a live C++ object at the resulting number.

## Bug to diagnose

Dereference the computed integer by casting it to a pointer. Numeric arithmetic has not proved that such an object exists.

Keep this model numeric. Use a valid array or OS mapping when the task actually requires reading storage.
