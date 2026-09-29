# Make subregister width an explicit input

Model writes of 16, 32, or 64 bits to a 64-bit x86 register. A 32-bit write clears the high half; a 16-bit write preserves the other bits. Reject unsupported widths.

## Acceptance checks

- Writing 7 at width 32 produces 7.
- Writing 7 at width 16 preserves the old high bits.
- Width 64 replaces the entire value.
- Width 8 is rejected by this deliberately limited model.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch13/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The write width is part of the operation, so the function receives it explicitly instead of guessing from the numeric value. Masking the new low bits prevents them from affecting preserved bits. Rejecting widths the model does not implement is more honest than silently applying a different rule. This models ISA behavior; it does not force the compiler to use a specific register.

## Bug to diagnose

Preserve the old high half for a 32-bit write. That models an AX-like partial update, not an EAX write.

Treat width 32 as zero extension of the low 32-bit value. Test with nonzero old high bits so the distinction is observable.
