# Avoid opposite lock-order deadlock

Transfer between accounts with two mutexes using scoped_lock. Treat transfer to the same account as a no-op, reject insufficient funds, and preserve the total.

## Acceptance checks

- Two workers transfer 100 units in opposite directions without opposite manual lock order.
- Total remains 200.
- Transfer to self changes nothing.
- For distinct accounts, a request greater than the source balance returns false.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch50/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

scoped_lock acquires the two distinct mutexes using a deadlock-avoidance strategy and releases both at scope exit. Handling the same-account case first avoids passing the same non-recursive mutex twice. The small fixed total bounds arithmetic in this fixture. Other locks, callbacks, or I/O added inside the critical section would introduce new waiting relationships that still need review.

## Bug to diagnose

Lock the source mutex first and destination second in both transfer directions. Each thread can hold the lock the other needs.

Use a consistent global ordering or an appropriate multi-mutex acquisition operation. Separately handle repeated references to the same resource.
