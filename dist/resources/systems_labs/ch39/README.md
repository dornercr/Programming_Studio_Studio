# Classify termination before reading its value

Use a tagged status model for normal exit and signal termination. Do not interpret a signal number as an exit code.

## Acceptance checks

- A normal exit value 7 formats as exit=7.
- A signal termination value 15 formats as signal=15.
- The tag controls the interpretation of the numeric field.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch39/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The enum says which interpretation of value is valid. A real wait status uses OS-defined macros to discover this category; its packed integer is not directly the exit code. Separating classification from formatting makes the result clear to callers and prevents accidental bit guesses.

## Bug to diagnose

Always print status.value as an exit code. A signal-killed child is then reported as if it returned normally.

Classify the termination kind first, then extract the corresponding value using the platform API. Do not invent a universal bit layout.
