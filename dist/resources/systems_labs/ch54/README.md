# Report a distribution as well as a mean

For a nonempty list of finite nonnegative millisecond durations with positive total time, report mean, nearest-rank 95th percentile, and serial throughput.

## Acceptance checks

- 2,8,5 gives mean 5, p95 8, throughput 200 per second.
- A single 10 ms request gives p95 10 and throughput 100.
- Empty input, all-zero time, and negative durations are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch54/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The mean describes total time divided by count; the selected percentile describes an order statistic. Sorting a copy preserves the caller’s sample order. Three observations make a very weak basis for real tail-latency inference: the example teaches the calculation, not statistical confidence. Throughput here uses a serial interval; concurrent request durations cannot simply be added to infer wall time.

## Bug to diagnose

Report 3/15 as requests per second. The denominator is milliseconds, so the units are wrong by a factor of 1000.

Convert the interval to seconds and state the percentile convention. Preserve raw measurements for later checks.
