# Chapter 2: Check a measured environment report

Return true only if compiler and library are both available and the measured language tag is at least 202002. The function evaluates reported facts; it does not detect installed tools.

## Design and rules
A Boolean conjunction directly expresses the three independent requirements. Passing the facts makes boundary tests independent of this machine.

Success requires every stated fact, including the language-version boundary.

Alternative: An environment probe can gather facts, but mixing detection and evaluation makes controlled unit tests harder.

Starter defect: With an available compiler and tag 201703, the starter returns true even though the required language mode is absent.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch02/solution.cpp -o /tmp/book-one-ch02
printf '%s' '201703 1 1
' | /tmp/book-one-ch02
```
Expected standard output:
```text
false
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
