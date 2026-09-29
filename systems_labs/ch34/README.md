# Copy across short reads and writes

Build a deterministic transfer model that limits each read to two bytes and each write to one byte. Advance by actual counts and reject a zero-progress configuration.

## Acceptance checks

- ABCDE arrives intact even though every write is partial.
- Empty input produces empty output.
- A zero read or write limit is rejected rather than looping forever.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch34/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Input progress and output progress are separate counters. A buffer must be fully written before it can be discarded or overwritten by another read. The inner loop advances by the actual write count. Real descriptor code must distinguish EOF, interruption, would-block, and permanent errors; this model supplies positive bounded transfers to isolate the progress rule.

## Bug to diagnose

Advance sent by buffer.size() after the first one-byte write. Only A, C, and E reach the destination for this trace.

Advance by written and loop over the untransferred suffix. Test read and write limits that differ so a one-call assumption cannot hide.
