# Chapter 24: Do not accept an empty checklist

Return true only when at least one required check was supplied and every supplied result is true. An empty list is incomplete, not proof of acceptance. This models recorded checks; it does not execute the application tests.

## Design and rules
A nonempty check makes the evidence requirement explicit before a simple all-results scan.

Success means evidence exists and every supplied required check passed. It does not assert that unlisted requirements were tested.

Alternative: A report with named requirements and missing-result states is stronger for a real release. This Boolean summary is a small exercise about the acceptance boundary.

Starter defect: The starter returns true for an empty vector. The loop has no failing result, but the contract also requires actual evidence.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch24/solution.cpp -o /tmp/book-one-ch24
printf '%s' '' | /tmp/book-one-ch24
```
Expected standard output:
```text
false
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
