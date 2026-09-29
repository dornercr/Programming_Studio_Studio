# Compare split updates with atomic read-modify-write

Keep a deterministic split-load/store counterexample, then have two real workers perform 1000 atomic fetch_add operations each.

## Acceptance checks

- Two split loads followed by two stores finish at one.
- Two workers using fetch_add finish at 2000.
- The real counter is read for its final result only after both workers finish.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch45/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Every access to each shared atomic counter is atomic, but only fetch_add combines reading and updating into one indivisible operation. The split trace intentionally separates those steps and has a logical lost update without undefined behavior. Completion of both futures makes the final count meaningful; checking while workers run would observe a valid but incomplete total.

## Bug to diagnose

Replace fetch_add with load followed by store in each worker. The C++ accesses remain atomic, but increments can be lost.

Use one read-modify-write operation for the counter, or protect the entire logical update with a mutex when several objects must change together.
