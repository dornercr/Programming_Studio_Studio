# Chapter 17: Protect a stock invariant

Stock(int count) accepts 0 through 100 and otherwise throws std::invalid_argument("range"). count() const reports count. take(int amount) accepts 0 through available count, subtracts it, and returns true; rejection returns false unchanged.

## Design and rules
Private storage and validated operations place the count rule at one boundary. The const query lets callers inspect state without a write path.

Every successfully constructed Stock keeps count in 0 through 100. Failed takes preserve the count.

Alternative: A public int is enough for a local calculation with one trusted owner. It becomes weaker when several callers must preserve the same stock rule.

Starter defect: The starter rejects taking all five available units. It uses >= where the contract allows equality.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch17/solution.cpp -o /tmp/book-one-ch17
printf '%s' '' | /tmp/book-one-ch17
```
Expected standard output:
```text
true 0
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
