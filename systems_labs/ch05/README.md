# Compare nearby floating-point results

Implement a relative-and-absolute closeness test for finite doubles. Make the tolerances explicit and reject negative tolerances. Exact equality handles equal infinities before the finite-only calculation.

## Acceptance checks

- 0.1+0.2 and 0.3 are close under 1e-12 tolerance.
- A small absolute tolerance works near zero.
- Distinct large values beyond the selected relative tolerance are not close.
- NaN is not close to any value; negative tolerances are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch05/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

An absolute tolerance gives a useful bound near zero; a relative tolerance scales with magnitude. The application chooses both from its error budget. Closeness is not a universal equivalence relation: it can fail transitivity, so it must not replace ordering in a sorted container. This lab covers modest finite magnitudes; a fully general comparator must also avoid overflow in difference and tolerance calculations.

## Bug to diagnose

Use a fixed epsilon as a universal rule for all magnitudes. A bound useful near 1 can be too tight or too loose elsewhere.

State an error budget and test both near-zero and scaled values. Keep NaN handling explicit rather than letting an accidental comparison decide policy.
