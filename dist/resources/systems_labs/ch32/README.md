# Use local miss rates in a two-level cost model

Compute h1 + m1×(h2 + m2×memory). Miss rates must lie in [0,1]; times must be nonnegative. Assume serial costs and incremental penalties.

## Acceptance checks

- h1=1,m1=.1,h2=5,m2=.2,memory=50 gives 2.5.
- No L1 misses gives h1 alone.
- Rates outside [0,1] are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch32/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Only L1 misses reach L2, and only the fraction m2 of those pay the memory penalty. Multiplying the rates preserves their denominators. The h1 and h2 terms are incremental lookup costs in this model; adding a miss penalty that already contains them would count time twice. Real overlapping accesses require a different performance interpretation.

## Bug to diagnose

Use h1+m1*h2+m2*memory. It treats the L2 local miss rate as a fraction of all original accesses.

Nest the L2 cost inside the L1 miss probability. Write the denominator of every rate before substituting numbers.
