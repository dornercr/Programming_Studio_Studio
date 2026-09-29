# Grow a collection under a hard limit

Append an element only when a vector is below a fixed maximum size. Rejection must leave the vector unchanged. Do not retain an element pointer while growing it.

## Acceptance checks

- A size-two vector accepts its third element with maximum three.
- The next append is rejected without changing existing values.
- A maximum of zero rejects even the first element.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch09/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The limit is a bound on live elements, not on reserved capacity. A rejection occurs before push_back, so it cannot partially alter the sequence. The vector handles storage growth and release. This function may still throw if allocation fails; its Boolean reports only the explicit size-limit decision.

## Bug to diagnose

Compare capacity() with maximum. Reserving spare storage can then reject a valid append or confuse storage with live elements.

Use size() for an element-count rule. Keep allocation failures distinct from an intentional capacity policy.
