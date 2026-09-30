# Partition an uneven range safely

Sum a vector with two asynchronous workers, splitting at size/2. Keep the input alive until both results are obtained and support empty and odd-length inputs.

## Acceptance checks

- {1,2,3,4,5} totals 15 with no lost element.
- An empty vector totals zero.
- One element is assigned to exactly one partition.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch44/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The half-open ranges [0,middle) and [middle,size) meet without overlap or a gap. Futures return values, so workers need no shared mutable accumulator. get waits for completion and propagates worker exceptions. The input remains borrowed until both asynchronous tasks finish; this lab assumes the small input sums fit in long long. Parallelism is an implementation exercise, not a speed claim.

## Bug to diagnose

Give each worker size/2 elements starting at worker*(size/2). For five elements, the fifth is omitted.

Use the actual size as the second range’s end. Check empty, one-element, even, and odd sizes.
