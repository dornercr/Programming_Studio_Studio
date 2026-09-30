# Separate calculation from presentation

Extract a reusable sum operation and a formatter. Changing the label must not change the numeric result. Inputs are small integers whose sum fits in int.

## Acceptance checks

- sum(7,5) is 12; sum(-2,5) is 3.
- The formatter accepts a caller-supplied label.
- Both formatted results include the computed value.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch01/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The calculator returns a value and knows nothing about streams. The formatter turns that value into owned text. Tests can check each boundary independently; a different output destination need not alter addition. The compiler may inline both functions, so this source-level separation does not promise extra machine calls.

## Bug to diagnose

Put the label into the arithmetic function and return only formatted text. A numeric caller then has to parse the text again.

Return an int from calculation and create text at the presentation boundary. Keep the input range explicit; these tests do not prove signed addition cannot overflow for arbitrary inputs.
