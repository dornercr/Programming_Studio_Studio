# Read a bounded record slice

Return a substring only when offset and length fit within the file model. An empty slice at end is valid; an offset past end is not.

## Acceptance checks

- ABCDE at offset two,length three gives CDE.
- Offset five,length zero gives an empty string.
- Past-end offset and oversized length are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch33/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The slice owns its returned characters, so it does not depend on the original string’s later lifetime. The checks use the valid input extent, not its allocation capacity. An optional empty string is a success, while an empty optional is rejection. Real file reads can also fail or return fewer bytes; this memory model isolates the bounds contract.

## Bug to diagnose

Treat an empty returned string as failure. A valid zero-length slice then becomes indistinguishable from rejection.

Represent the success state separately from its data. Test the boundary at exactly end and just beyond end.
