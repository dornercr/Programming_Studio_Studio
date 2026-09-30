# Validate a complete bounded frame extent

Return whether a header and claimed payload fit inside a received extent, with a separate maximum payload of eight. Check the header before subtracting.

## Acceptance checks

- Header 2,payload 3,received 5 is valid.
- A missing header and a truncated payload are invalid.
- A payload above eight is invalid even if the buffer is larger.
- A maximum-size claim cannot wrap past the checks.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch57/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A parser needs both a received-extent check and an application resource limit. A large backing allocation does not imply that unread bytes are valid input, and a valid extent does not authorize an arbitrarily large message. Short-circuit evaluation ensures subtraction occurs only after the header is known to fit.

## Bug to diagnose

Compare against buffer capacity rather than the number of bytes actually received. Uninitialized or stale bytes can be mistaken for input.

Carry valid extent with the data and validate against it. Keep allocation capacity as a separate property.
