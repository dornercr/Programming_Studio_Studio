# Compare batching with separate transfers

Under a serial transfer model, compare one 10,000-byte batch with 100 transfers of 100 bytes. Latency is 10 microseconds and bandwidth is 100 bytes per microsecond; reject nonpositive bandwidth.

## Acceptance checks

- The batch costs 110 microseconds.
- Separate requests cost 1100 microseconds.
- Zero and negative bandwidth are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch27/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Each separate transfer pays startup latency again. A larger batch pays it once, so batching can help even without faster hardware. The result relies on serialization, fixed latency, fixed bandwidth, and finite modest inputs. Real queueing, overlap, contention, and protocol limits can change the result.

## Bug to diagnose

Use bytes*bandwidth for the transfer time. The units become bytes squared per time rather than time.

Divide byte count by bytes per microsecond. Carry units through the arithmetic before trusting the number.
