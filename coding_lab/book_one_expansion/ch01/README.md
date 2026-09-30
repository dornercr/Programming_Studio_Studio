# Chapter 1: Name a build-pipeline stage

Map 0 to preprocess, 1 to compile, 2 to assemble, and 3 to link. Return invalid for any other integer. This models stage labels; it does not run a compiler.

## Design and rules
Explicit comparisons expose the four-stage mapping without introducing a new type. The driver prints the returned label.

Each valid stage has exactly one label; every other integer is rejected with invalid.

Alternative: A switch is equally reasonable. A lookup table is useful for larger mappings but still needs a bounds check.

Starter defect: For stage 3 the starter returns compile instead of link. It also mislabels every invalid nonzero input.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch01/solution.cpp -o /tmp/book-one-ch01
printf '%s' '3
' | /tmp/book-one-ch01
```
Expected standard output:
```text
link
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
