# Translate through a checked page map

Translate byte addresses using 4096-byte pages and a virtual-page-to-frame map. Reject an unmapped virtual page and guard the frame multiplication.

## Acceptance checks

- 0x1234 maps through virtual page 1 to physical 29236 when frame is 7.
- The next byte maps to 29237.
- An unmapped page returns no result.
- An oversized frame number is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch23/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The quotient selects a mapping and the remainder stays unchanged. A failed lookup is a defined outcome, not an accidental insertion of frame zero. This lab checks numeric mapping and overflow; permissions are added in the next chapter. A real OS may resolve some missing entries rather than simply reject them.

## Bug to diagnose

Use pages[page] when only looking up a map. That operation can insert a default entry and make an unmapped page appear to map to frame zero.

Use find for a read-only lookup and handle end explicitly. Keep absence distinct from a valid mapping to frame zero.
