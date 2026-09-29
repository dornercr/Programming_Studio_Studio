# Guard a conservation rule across real workers

Two accounts begin with 100 each. Two workers make 100 opposite-direction transfers of one unit. Protect each complete transfer with the same mutex.

## Acceptance checks

- Every completed transfer preserves total 200.
- Both workers finish with balances 100 and 100.
- No balance is read or changed concurrently outside the common lock.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch47/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The invariant spans two variables, so the critical section spans both updates. The references from and to select direction while the lock is held. Each worker sends at most its account’s initial 100 units, so the positive-source assertion holds for every schedule in this fixture. Main reads the final state after both workers finish.

## Bug to diagnose

Give each worker its own mutex. Both can then enter their critical sections at the same time and race on the accounts.

Use the same synchronization object for every access governed by the invariant. Protect readers as well as writers if they can run concurrently.
