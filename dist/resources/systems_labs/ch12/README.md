# Turn the instruction trace into a bounded interpreter

Interpret Load, Add, and Halt in a tiny model. Reject a program that runs past its instruction array without halting. Arithmetic inputs are limited to small values with representable results.

## Acceptance checks

- LOAD 4, ADD 3, HALT returns 7.
- HALT alone returns the initial zero.
- A program without HALT is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch12/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

An enum gives each modeled operation a distinct name. A small struct now earns its place because an instruction consists of both an operation and an operand. The switch defines the instruction semantics while the vector traversal supplies the next instruction. The model is deliberately not a binary encoding or a real processor pipeline.

## Bug to diagnose

Fall through from Load into Add by omitting break. LOAD 4 then leaves 8 rather than 4 in the accumulator.

End each non-returning case explicitly and test a one-instruction effect before testing a whole program.
