# Distinguish data, waiting, and EOF

Return a pipe-read state from buffered-byte count and open-writer count. Buffered data must be delivered even after the last writer closes.

## Acceptance checks

- Buffered data takes priority over EOF.
- An empty buffer with an open writer means wait in this blocking model.
- An empty buffer with no writers means EOF.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch40/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

EOF describes a drained stream whose producer ends are closed. Closing the last writer does not erase already-buffered bytes. The three-state result prevents an empty buffer from being confused with a finished stream. A nonblocking real pipe would report would-block rather than waiting when writers still exist but no data is available.

## Bug to diagnose

Return EOF as soon as writer count reaches zero. This discards the final buffered bytes.

Check for available data before checking whether further data can arrive. Test a closed pipe that still has buffered bytes.
