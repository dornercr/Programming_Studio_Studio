# Enforce scheduler state transitions

Use an enum to model Ready, Running, and Waiting. Permit dispatch, blocking, and wakeup only from their valid starting states.

## Acceptance checks

- Ready→Running dispatch succeeds.
- Running→Waiting block succeeds.
- Waiting→Ready wakeup succeeds.
- Dispatching a waiting process is rejected without changing it.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch21/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A state machine makes the permitted change explicit at each call. Checking the source state prevents an event from pretending a blocked process can run. This generic transition helper trusts its caller to supply the allowed edge; a production scheduler would expose named operations or validate the edge set centrally.

## Bug to diagnose

Wake a waiting process directly into Running without considering which process owns the CPU.

Wakeup makes work eligible by moving it to Ready. Dispatch is a separate scheduling decision.
