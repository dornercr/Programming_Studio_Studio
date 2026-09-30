# Chapter 23: Trace cleanup of incomplete construction

Record member construction A+, B+, then owner body. On success record owner-, B-, A-. If the owner body throws, record B-, A-, caught with no owner- entry. Return the entries joined by single spaces.

## Design and rules
The log outlives the owner and its members. Actual constructors and destructors produce the trace, so the evidence follows the language cleanup rules.

Completed members are destroyed in reverse order. An owner whose constructor throws never runs its own destructor.

Alternative: A hard-coded trace can predict one case but does not demonstrate real cleanup. Production cleanup should use a nonthrowing logging policy; this bounded test assumes log allocation succeeds.

Starter defect: The starter includes owner- on failure, claiming destruction of an object that never completed construction.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch23/solution.cpp -o /tmp/book-one-ch23
printf '%s' '' | /tmp/book-one-ch23
```
Expected standard output:
```text
A+ B+ body B- A- caught
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
