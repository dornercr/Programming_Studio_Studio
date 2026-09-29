# Enforce an admission budget

Create a gate with a maximum active count. Admit only below the limit, release one existing permit at a time, and reject a release when none is held.

## Acceptance checks

- A limit-two gate admits twice and rejects a third request.
- Releasing one permit allows another admission.
- Active count never exceeds two or falls below zero.
- An unmatched release is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch52/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Checking the count and incrementing it form one critical section, otherwise two arrivals could both observe spare capacity. The gate bounds admitted work, not every byte the server owns. Callers must still pair successful admission with release on every completion and error path; a scope-based permit object is a useful next extension.

## Bug to diagnose

Check the count without a lock, then lock only while incrementing. Two callers can both pass the same old count.

Guard the check and state change together. Treat cleanup pairing as part of the admission contract.
