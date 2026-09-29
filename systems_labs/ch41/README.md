# Separate pending state from an event count

Model both a Boolean notification and a counted event queue. Deliver two events before consuming anything, then explain why the two representations produce different amounts of information.

## Acceptance checks

- Two Boolean notifications leave one true flag.
- Two counted events leave count two.
- Consuming the flag once clears it; consuming one count leaves one event.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch41/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A flag answers whether attention is needed; a counter can preserve multiplicity within its range. Neither this ordinary bool nor unsigned is a real concurrent signal-handling protocol. A real handler must obey async-signal-safety rules, and the main loop must close the check-then-sleep race using the OS’s waiting and mask operations.

## Bug to diagnose

Expect two standard signal notifications to behave like two items in a reliable work queue.

Treat a signal as a prompt to inspect authoritative state unless the API explicitly queues the required information. Use a suitable counted mechanism when every event matters.
