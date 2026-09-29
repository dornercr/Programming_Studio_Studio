# Show both detection and its limit

Model a record with payload and guard. Demonstrate that changing the guard is detected while a payload-only change can leave the guard check satisfied. Do not perform an invalid memory access.

## Acceptance checks

- The initial guard passes.
- A deliberate guard-variable change fails the check.
- Changing only the payload leaves the guard check passing.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch58/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A check proves only the fact it actually checks. This guard comparison can notice a changed guard value; it does not validate payload semantics or enforce bounds. The deliberate assignments are legal C++ operations, so the demonstration does not rely on undefined overflow behavior. Real mitigations add distinct defenses but do not replace a correct memory-access contract.

## Bug to diagnose

Treat a passing guard check as proof that no memory-safety bug occurred anywhere in the operation.

State the mitigation’s coverage and test its limitations. Keep length validation, lifetime rules, and build hardening as separate protections.
