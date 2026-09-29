# Build reusable bounded range sums

Precompute prefix sums for at most one million int values in [-1000,1000]. Answer half-open range queries and reject reversed or out-of-bounds endpoints.

## Acceptance checks

- [1,4) over 2,4,6,8 returns 18.
- An empty range returns zero.
- [0,4) returns 20.
- Reversed endpoints and endpoints beyond size are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch55/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A leading zero makes the same subtraction work for ranges starting at zero and for empty ranges. The bounds on count and element value keep every prefix sum representable in long long. Queries borrow an already-built valid prefix vector; exposing an arbitrary editable vector is a deliberate simplification that a value type could later protect.

## Bug to diagnose

Build a prefix array without the leading zero but keep the same query formula. Endpoint zero and the full-range query no longer follow the stated meaning.

Define sums[k] as the total strictly before k and keep that meaning in construction, bounds, and queries.
