# Enumerate defined interleavings

Enumerate every order of two read/write steps from A and two from B while preserving each participant’s read-before-write order. Count final values in this sequential model.

## Acceptance checks

- There are six distinct schedules.
- Four lose an update and finish at one.
- Two serialize the updates and finish at two.
- No actual unsynchronized C++ threads are used to model the failure.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch43/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Each participant’s first event reads and its second writes. Permuting the actor labels enumerates only schedules that keep this internal order. The sequential simulator has defined behavior, so its outcomes can be counted and inspected. Those counts do not assign probabilities to a real scheduler and do not describe the full set of behaviors of a C++ data race.

## Bug to diagnose

Run ordinary shared counter++ from two threads and claim the observed results enumerate all valid outcomes.

Use a defined model for interleaving reasoning, or synchronize the real program. Undefined behavior is not a scheduling model.
