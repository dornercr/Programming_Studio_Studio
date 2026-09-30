# Compare FIFO and LRU with the same trace

Simulate a two-entry cache with either first-in-first-out or least-recently-used replacement. Return the final contents from oldest to newest under the selected policy.

## Acceptance checks

- Trace A,B,A,C leaves B,C under FIFO.
- The same trace leaves A,C under LRU.
- Repeated hits do not create duplicate entries.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch31/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The container order is the policy state. FIFO leaves it unchanged on a hit; LRU moves a hit to the recent end. Both insert a new block at the end and evict from the front when full. Using the same trace isolates the policy difference; it does not prove one policy wins on every workload.

## Bug to diagnose

Move a hit to the end in the FIFO branch. The implementation no longer preserves arrival order.

Update recency only for LRU. Use a hit between insertion and eviction to distinguish the policies.
