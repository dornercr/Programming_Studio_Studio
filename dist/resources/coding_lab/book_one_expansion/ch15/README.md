# Chapter 15: Build an automatically owned sequence

For count 1 through 8 return an owning int array with values 1 through count. For zero return an empty owner. Above 8 throw std::invalid_argument("range"). The caller keeps the count separately.

## Design and rules
unique_ptr<int[]> makes sole array ownership explicit. Scope-based destruction releases the array even if later caller work throws.

The returned owner manages exactly count elements; allowed indices are below count. No raw delete is required from the caller.

Alternative: vector<int> is usually simpler when storing size and collection operations is needed. This exercise isolates ownership transfer from the size policy.

Starter defect: The starter returns an allocated array containing 0 0 0 for the sample. Owning storage correctly does not establish the required element values.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch15/solution.cpp -o /tmp/book-one-ch15
printf '%s' '' | /tmp/book-one-ch15
```
Expected standard output:
```text
1 2 3
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
