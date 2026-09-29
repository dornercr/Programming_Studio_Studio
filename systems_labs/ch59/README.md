# Validate then commit a configuration update

Accept exactly version=N with decimal N in [1,99]. Parse into temporary state and change the live string only after complete validation.

## Acceptance checks

- version=2 replaces version=1.
- A missing number, trailing junk, zero, or 100 is rejected.
- Every rejected update preserves the previous live configuration.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch59/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

from_chars reports both an error and the point where parsing stopped, so accepting a numeric prefix alone is not enough. The candidate must fit the entire grammar and range. A separately built string provides a commit point through swap; invalid input leaves the old value untouched. This is process-local exception-safe state replacement, not a durable file transaction.

## Bug to diagnose

Accept version=2x because the numeric parser found an initial 2. The unconsumed suffix violates the format.

Require successful conversion and parsed.ptr==last, then enforce the range before committing.
