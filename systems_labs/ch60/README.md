# Build a byte-summary component with a clear contract

Summarize a string of up to one million bytes by its byte count and unsigned-byte sum. Treat embedded zero bytes as data and reject oversized input.

## Acceptance checks

- cat has three bytes and sum 312.
- A two-byte string containing 0 and 255 has sum 255.
- Empty input is valid.
- The byte-count cap is enforced.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch60/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A string stores a count as well as characters, so an embedded null does not end this range-based traversal. Converting each element to unsigned char makes the byte value nonnegative. The length cap bounds the sum well below uint64_t’s maximum. The lab isolates a computation that can later be used behind a file, pipe, or network input boundary.

## Bug to diagnose

Use strlen on the binary input. It stops at the first zero byte and reports zero rather than two.

Carry an explicit byte count and iterate the whole valid range. Keep text terminators separate from binary framing.
