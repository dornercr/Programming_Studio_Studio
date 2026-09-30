# Chapter 3: Read one whole bounded count

Accept exactly one integer from 0 through 20, with optional surrounding stream whitespace. Reject empty, malformed, out-of-range, and trailing non-whitespace text. On rejection leave output unchanged.

## Design and rules
A temporary separates parsing from committing caller state. A stream matches the whitespace rule and the beginner input lesson.

A rejected field preserves the old output; an accepted field consumes the whole permitted field and satisfies 0 through 20.

Alternative: from_chars is useful for strict byte-range parsing, but it has different whitespace rules that the adapter must handle explicitly.

Starter defect: For 12x the starter reports success and changes output to 12. It neither validates the whole input nor preserves state on all failures.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch03/solution.cpp -o /tmp/book-one-ch03
printf '%s' '12x
' | /tmp/book-one-ch03
```
Expected standard output:
```text
invalid 7
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
