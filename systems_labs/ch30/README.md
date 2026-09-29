# Make cold-cache behavior a reusable function

Simulate reads for a configurable direct-mapped cache. Each call begins cold; use explicit empty slots and return the hit count. Reject zero line size or zero slot count.

## Acceptance checks

- The chapter trace has two hits with four 16-byte slots.
- An empty trace has zero hits.
- Reading the same address three times has two hits.
- Zero geometry is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch30/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

optional gives each slot an empty state separate from every unsigned block number, including zero. The vector is local to the simulation call, so repeated tests start cold rather than sharing hidden history. On a miss, the selected slot is replaced; the other slots keep their state. The function reports events, not elapsed time.

## Bug to diagnose

Initialize unsigned slots to zero and compare only the stored value. The first access to block zero is incorrectly called a hit.

Represent validity explicitly, with optional or a valid bit, and include block zero as the first cold access in a test.
