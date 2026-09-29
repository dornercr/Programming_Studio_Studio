# Parse complete frames without losing the next one

Parse a one-byte length followed by that many payload bytes, with maximum length eight. Report incomplete input separately from an invalid length, and return the number of consumed bytes.

## Acceptance checks

- A fragmented cat frame stays incomplete until all three payload bytes arrive.
- A complete frame reports four consumed bytes.
- A following frame remains available to parse.
- A length above eight is rejected before payload allocation.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch51/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A byte stream can split or combine application frames arbitrarily. The parser therefore returns both owned payload data and a consumed count. It does not erase unconsumed bytes itself. Bounds are checked before forming payload iterators, and invalid size is distinct from incomplete arrival. Network errors and end-of-stream handling belong to the surrounding connection state machine.

## Bug to diagnose

Clear the whole receive buffer after parsing the first frame. Any complete or partial second frame disappears.

Remove only consumed bytes and retain the suffix. Test two frames arriving in one read as well as one frame arriving in several reads.
