# Traverse empty and nonempty ranges

Implement a bounded sum using vector iterators. Use a wide accumulator for this small-int fixture and support an empty range without dereferencing its end.

## Acceptance checks

- {3,5,7} sums to 15.
- The empty vector sums to zero.
- {-4,4} sums to zero.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch07/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A half-open range stops before its end iterator. For an empty vector, begin equals end, so no dereference occurs. The const reference prevents this function from resizing the vector. That matters because reallocation could invalidate its iterators. A wider accumulator reduces overflow risk for small fixtures but does not make an unbounded sum safe.

## Bug to diagnose

Use <= instead of != on random-access iterators in this loop. It tries to dereference end.

Stop at end, and test the empty range before trusting a traversal. There is no valid element to read from an empty range.
