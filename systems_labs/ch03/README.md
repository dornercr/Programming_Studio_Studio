# Validate a packed-field setter

Create a setter for bits 4–7 of an unsigned word. Reject values above 15 and preserve all bits outside that field.

## Acceptance checks

- 0xA5 with field 3 becomes 0x35.
- Writing 0 and 15 clears or fills the selected field.
- 16 is rejected and neighboring bits stay unchanged.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch03/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The setter clears the old four-bit field before inserting the new one. The two groups do not overlap, so bitwise OR combines them without carrying. Unsigned operations avoid sign-extension questions. The check belongs inside the setter because every caller must obey the same width rule.

## Bug to diagnose

Use word | (value << 4) without clearing. Setting 0xA5 to field 3 produces 0xB5.

Clear with word & ~mask, then insert with OR. Test clearing to zero, which immediately exposes a setter that can only add one-bits.
