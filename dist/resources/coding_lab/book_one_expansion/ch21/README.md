# Chapter 21: Identify the earliest failed boundary

Given three recorded exit statuses, report compile if compile is nonzero, otherwise link if link is nonzero, otherwise run if run is nonzero, otherwise none. Later statuses are ignored after an earlier failure.

## Design and rules
An ordered guard chain encodes diagnostic priority. It uses recorded status facts rather than guessing from the final output.

The first failed boundary owns the diagnosis; successful later-looking evidence cannot override it.

Alternative: A structured report retaining all statuses is useful for tooling. Its summary should still identify the earliest actionable failure.

Starter defect: The starter reports run for three failed statuses. That skips the compile failure that must be resolved before later evidence is meaningful.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch21/solution.cpp -o /tmp/book-one-ch21
printf '%s' '1 1 1
' | /tmp/book-one-ch21
```
Expected standard output:
```text
compile
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
