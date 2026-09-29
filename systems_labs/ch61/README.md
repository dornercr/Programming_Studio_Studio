# Capstone: bounded concurrent byte processing

Integrate a capacity-two work queue, two workers, byte validation, owned payloads, ordered results, and shutdown. Accept payloads of at most four bytes; rejected items must receive a result without contributing to totals.

## Acceptance checks

- cat, A, empty, tools produce three accepted results and one rejection.
- Accepted totals are four bytes and byte sum 377.
- Results are printed in input order regardless of worker schedule.
- An empty input list shuts down cleanly.
- Workers finish before the queue and result storage leave scope.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch61/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Work items own their strings, so queue operations do not leave borrowed input pointers behind. Main assigns each index once; workers therefore write distinct result elements without resizing the vector. Queue predicates and close are protected by one mutex. Every normal and exceptional path closes the queue and collects workers before local shared state dies. Main prints after collection, so scheduling does not reorder the report. Queue capacity bounds queued jobs; it does not bound the already-loaded input vector’s total memory. A streaming input layer would need its own size and backpressure contract.

## Bug to diagnose

Let workers push_back into the shared results vector without a lock, or let main return before collecting workers. The first can race on container internals; the second can leave dangling references.

Allocate the result slots before starting workers, assign each slot to one input, and keep all shared objects alive through worker completion. Preserve close-and-collect cleanup if any operation fails.
