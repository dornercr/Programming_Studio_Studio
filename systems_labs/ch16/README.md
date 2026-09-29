# Generalize a loop with a clear stopping rule

Sum integers in [0,limit) for limits up to 1000. Reject larger limits so the accumulator’s range is easy to justify.

## Acceptance checks

- Limit zero returns zero without entering the body.
- Limits one and four return zero and six.
- Limit 1000 returns 499500; 1001 is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch16/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The invariant before each loop test is that total contains the sum of all indexes less than i. The body adds i once, and the increment moves the boundary forward. At termination i equals limit. The stated cap keeps the result representable on the required implementation, which this lab checks with a static assertion in the generated program if needed.

## Bug to diagnose

Use i <= limit. For limit four the result becomes ten instead of six.

Use the half-open condition i < limit. Include limit zero in the tests to expose extra-iteration mistakes.
