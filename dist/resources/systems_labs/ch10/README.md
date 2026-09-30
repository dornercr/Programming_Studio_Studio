# Build a bounded aligned arena model

Implement reserve_bytes(used,capacity,request) for eight-byte-aligned starts. Return no start on failure and leave used unchanged. This models offsets, not actual object construction.

## Acceptance checks

- Requests 5 and 9 start at offsets 0 and 8.
- A request that crosses the capacity is rejected.
- A failed request leaves used unchanged.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch10/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Padding is computed separately and checked against remaining space before addition. The state changes only after every bound passes, which makes failure leave the arena usable. Returning an optional offset distinguishes rejection from a valid start at zero. This bump model has no individual free operation and does not create C++ objects in raw storage.

## Bug to diagnose

Update used before checking the request. A rejected allocation then consumes or corrupts the remaining arena state.

Compute candidate offsets in local values and commit used only after the last bound check. Test failure after one successful allocation.
