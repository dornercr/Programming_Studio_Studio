# Publish a caller-supplied payload

Build a one-shot publication function. A worker writes a non-atomic payload then releases a ready flag; the caller acquires that flag before reading the payload.

## Acceptance checks

- Publishing 42 returns 42.
- Publishing a negative small integer preserves its value.
- Each invocation uses a fresh flag and payload.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch46/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The release store follows the payload write; the acquiring load that observes it orders the subsequent payload read. The local state outlives the producer because the future is completed before return. A fresh flag makes the protocol one-shot. Reusing one Boolean for several messages would need a richer protocol to prevent missed or overwritten publications.

## Bug to diagnose

Use relaxed ordering on both flag operations and still rely on the flag to publish a non-atomic payload.

Use the release/acquire handoff or a mutex-based protocol that supplies the required happens-before relation. Do not treat a successful test run as a memory-order proof.
