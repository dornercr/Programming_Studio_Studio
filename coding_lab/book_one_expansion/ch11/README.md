# Chapter 11: Require complete numeric fields

Use from_chars to parse one complete decimal int field, with no whitespace or leading plus. Allow a leading minus only for zero (such as -0); the value must be 0 through 100. Reject an empty or partial field and preserve output on failure.

## Design and rules
from_chars exposes conversion status and the stopping position. A temporary protects the prior output from rejected text.

Success consumes every byte and produces 0 through 100; failure preserves the previous output.

Alternative: Stream extraction supports a different whitespace policy. It still needs complete-field and domain checks to meet this stricter contract.

Starter defect: The starter accepts 42x and commits 42. Conversion success alone proves only that a numeric prefix was read.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch11/solution.cpp -o /tmp/book-one-ch11
printf '%s' '42x
' | /tmp/book-one-ch11
```
Expected standard output:
```text
invalid 8
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
