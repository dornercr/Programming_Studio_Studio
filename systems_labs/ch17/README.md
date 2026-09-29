# Represent a call contract in C++

Write a helper that doubles a small integer and a caller that preserves a separate value across that call. Test positive, zero, and negative inputs.

## Acceptance checks

- caller(10,3) is 16.
- caller(10,0) is 10.
- caller(10,-3) is 4.
- The caller’s saved value is never overwritten by the helper.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch17/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The caller relies on a returned value and its own preserved value. The C++ compiler chooses how to satisfy that rule under the target ABI, possibly by eliminating the machine call entirely. Studying callee-saved registers requires inspecting a concrete build; the source-level contract is still the same if the helper is inlined.

## Bug to diagnose

In handwritten assembly, keep saved only in a caller-saved register and assume a nested call preserves it.

Follow the actual ABI’s preservation duties or let ordinary C++ express the call. A source-variable name is not a register-preservation promise.
