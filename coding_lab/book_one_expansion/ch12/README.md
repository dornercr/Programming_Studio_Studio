# Chapter 12: Append only inside a logical size limit

Append value only when it is -100 through 100 and size is below limit. Otherwise return false unchanged. limit is at most 100. Allocation failures may propagate; they are not represented by false.

## Design and rules
A size check enforces the user-visible bound before push_back. Capacity stays an implementation storage fact.

A false return preserves values and size. Successful append adds exactly one allowed element while staying within limit.

Alternative: A fixed std::array is better for a truly fixed extent. Here the logical count varies, so a vector fits the requirement.

Starter defect: The starter appends at size equal to limit. It reports true and size 3 for a limit of 2.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch12/solution.cpp -o /tmp/book-one-ch12
printf '%s' '' | /tmp/book-one-ch12
```
Expected standard output:
```text
false 2
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
