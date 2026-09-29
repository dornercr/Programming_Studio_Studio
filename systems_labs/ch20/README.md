# Distinguish recoverable and fatal faults

Model a load with separate permitted and present flags. A permitted absent page may be resolved and retried; a forbidden access leaves the program counter unchanged and returns failure.

## Acceptance checks

- Present,permitted load completes without a fault.
- Absent,permitted load records one fault then advances once.
- Forbidden load fails and never advances the program counter.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch20/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The model separates permission from presence. Missing backing is recoverable only when the mapping policy allows the access. The instruction advances after success, preserving the restart point across a repair. This is a model of architectural control transfer, not a real exception handler or a promise that every page fault is repaired.

## Bug to diagnose

Advance pc before checking the load. On denial, the model silently skips an instruction that never completed.

Commit the new program counter only on the successful path. Test both denied and recoverable cases from the same starting pc.
