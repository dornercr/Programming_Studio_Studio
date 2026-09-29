# Validate an Amdahl calculation

Compute whole-program speedup from a fraction in [0,1] and a finite positive local speedup. Reject invalid fractions and nonpositive factors.

## Acceptance checks

- Fraction .4 and local factor 2 gives 1.25.
- Fraction zero gives one; fraction one gives the local factor.
- A factor below one correctly predicts a slowdown.
- Invalid fractions and factor zero are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch56/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Normalize the old execution time to one, reduce only the affected fraction, and invert the new time to express a speed ratio. The model assumes the workload and unaffected costs stay fixed. Moving work across boundaries, changing input sizes, or adding coordination can invalidate the measured fraction even when the algebra is correct.

## Bug to diagnose

Use 1 + fraction*(local-1). That averages speed factors instead of adding execution times.

Combine time contributions first, then take their reciprocal. Check the fraction-zero and fraction-one boundaries.
