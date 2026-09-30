# Return a checked byte count

Write checked_bytes(count,width,limit). Return no value when multiplication would exceed limit; zero-width requests return zero.

## Acceptance checks

- 8×12 with limit 100 returns 96.
- 9×12 is rejected.
- Zero count and zero width are accepted as zero.
- The maximum size_t count times two is rejected against the type maximum.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch04/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The check divides before multiplying, so the product is formed only when it is within the caller’s limit. optional distinguishes a successful zero from rejection. A checked byte count still does not promise memory is available: allocation failure is a separate boundary.

## Bug to diagnose

Multiply first and compare the result with limit. An unsigned wrapped result can be small enough to pass.

When width is nonzero, require count <= limit/width before forming the product. Use a separate state to represent rejection.
